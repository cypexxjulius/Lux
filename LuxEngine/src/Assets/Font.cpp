#define USED_IN_FONTCPP
#include "Font.h"


#include "Core/IO.h"

#include "Utils/Assert.h"
#include "Utils/Logger.h"

#include <msdfgen.h>
#include <msdfgen-ext.h>
#include <msdf-atlas-gen/msdf-atlas-gen.h>

#include <stb_rect_pack.h>

using namespace msdf_atlas;

namespace Lux
{

void* Font::s_FreetypeLibraryHandle = nullptr;

void Font::Init()
{
	Verify(s_FreetypeLibraryHandle == nullptr);
	s_FreetypeLibraryHandle = static_cast<void*>(msdfgen::initializeFreetype());

	Verify(s_FreetypeLibraryHandle);
}

void Font::Shutdown()
{
	Verify(s_FreetypeLibraryHandle != nullptr);
	msdfgen::deinitializeFreetype(static_cast<msdfgen::FreetypeHandle*>(s_FreetypeLibraryHandle));
}

Font::Font(const String& filepath)
: m_Bitmap(nullptr)
{
	Verify(s_FreetypeLibraryHandle);
	
	auto *font = msdfgen::loadFont(static_cast<msdfgen::FreetypeHandle*>(s_FreetypeLibraryHandle), filepath.c_str());

	Verify(font);	
	
	if (!font)
		ERROR("Failed to open font {}", filepath);

	List<GlyphGeometry> glyphs;
	FontGeometry fontGeometry(&glyphs);

	fontGeometry.loadCharset(font, 1.0f, Charset::ASCII);

	msdfgen::FontMetrics metrics;
	msdfgen::getFontMetrics(metrics, font);

	float emsize			= static_cast<float>(metrics.emSize);
	m_LineHeight			= static_cast<float>(metrics.lineHeight) / emsize;
	m_AscenderY				= static_cast<float>(metrics.ascenderY) / emsize;
	m_DecenderY				= static_cast<float>(metrics.descenderY) / emsize;
	m_UnderlineThickness	= static_cast<float>(metrics.underlineThickness) / emsize;
	m_UnderlineY			= static_cast<float>(metrics.underlineY) / emsize;

	const double maxCornerAngle = 3.0;
	for(GlyphGeometry& glyph : glyphs)
		glyph.edgeColoring(&msdfgen::edgeColoringInkTrap, maxCornerAngle, 0);

	
	int image_width, image_height;
	{
		TightAtlasPacker packer;

		packer.setDimensionsConstraint(TightAtlasPacker::DimensionsConstraint::SQUARE);
		packer.setMinimumScale(32.0);

		packer.setPixelRange(2.0);
		packer.setMiterLimit(1.0);

		packer.pack(glyphs.data(), static_cast<int>(glyphs.size()));

		packer.getDimensions(image_width, image_height);
	}

	u32 image_size = image_width * image_height;
	Scope<byte[]> Buffer(new byte[image_size * 4]);
	{
		ImmediateAtlasGenerator<
			float, 
			3, 
			msdfGenerator,
			BitmapAtlasStorage<msdf_atlas::byte, 3>
		> msdf_generator(image_width, image_height);

		

		ImmediateAtlasGenerator< 
			float, 
			1, 
			scanlineGenerator,
			BitmapAtlasStorage<msdf_atlas::byte, 1>
		> generator(image_width, image_height);

		msdf_generator.setThreadCount(4);
		generator.setThreadCount(4);

		msdf_generator.generate(glyphs.data(), static_cast<int>(glyphs.size()));
		generator.generate(glyphs.data(), static_cast<int>(glyphs.size()));

		auto& msdf_storage = msdf_generator.atlasStorage();
		auto& storage = generator.atlasStorage();
	
		msdfgen::BitmapConstRef<msdf_atlas::byte, 3> msdf_bitmap = msdf_storage;
		msdfgen::BitmapConstRef<msdf_atlas::byte, 1> bitmap = storage;

		u32 pixel_count = 0;

		const byte* msdf_buffer = static_cast<const byte*>(msdf_bitmap.pixels);
		const byte* bitmap_buffer = static_cast<const byte*>(bitmap.pixels);
		
		for(u32 pixel_count = 0; pixel_count < image_size; pixel_count++)
		{
			Buffer[(pixel_count * 4)	] = msdf_buffer[(pixel_count * 3) + 0];
			Buffer[(pixel_count * 4) + 1] = msdf_buffer[(pixel_count * 3) + 1];
			Buffer[(pixel_count * 4) + 2] = msdf_buffer[(pixel_count * 3) + 2];
			Buffer[(pixel_count * 4) + 3] = bitmap_buffer[pixel_count];
		}
	}
	


	BitmapSpec spec;

	spec.unpack_aligned = true;
	spec.type = ImageType::RGBA;
	spec.mag_filter = FilterMethod::LINEAR;
	spec.min_filter = FilterMethod::LINEAR;

	m_Bitmap = Bitmap::Create(spec, image_width, image_height);
	m_Bitmap->set_data(static_cast<const void*>(Buffer.get()), image_size * 4);


	for (auto& glyph : glyphs)
	{
		/*
		(c,d)
			A
		(a,b) 

		*/


		double l, r, t, b;
		glyph.getQuadAtlasBounds(l, b, r, t);
		float glyph_left =	(float)(l / image_width);
		float glyph_width = (float)((r - l) / image_width);

		float glyph_bottom = (float)(b  / image_height);
		float glyph_height = (float)((t - b) / image_height);

		double advance = glyph.getAdvance();

		double x1, y1, x2, y2;
		glyph.getQuadPlaneBounds(x1, y1, x2, y2);

		float width = (float)(x2 - x1);
		float height = (float)(y2 - y1);

		float posx = (float)(x2 - width);
		float posy = ((float)(std::abs(y1) - height)) + m_LineHeight;

		auto cp =  glyph.getCodepoint();

		INFO("{} : ({}, {})", (char)cp, posx, posy);

		m_GlyphMetaData.insert({ 
			cp, 
			Glyph { 
				glm::translate(mat4(1.0f), { posx, posy, 0.0f }) * glm::scale(mat4(1.0f), { width, height, 1.0f }), 
				Array<v2, 4> {
					v2 { 
						glyph_left,				
						glyph_bottom 
					},
					v2 { 
						glyph_left + glyph_width,	
						glyph_bottom
					},
					v2 { 
						glyph_left + glyph_width,	
						glyph_bottom + glyph_height
					},
					v2 { glyph_left,				
						glyph_bottom + glyph_height
					}
				}, 
				advance
			}});
	}

	m_FontHandle = static_cast<void*>(font);
	
}
Font::~Font()
{
	msdfgen::destroyFont(static_cast<msdfgen::FontHandle*>(m_FontHandle));
}
;


};