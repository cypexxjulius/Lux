#type vertex
#version 440 core
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in float a_FontID;
layout(location = 4) in int a_UseMSDF;


out vec4 v_Color;
out vec2 v_texCoord;
out flat float v_FontID;
out flat int v_UseMSDF;


uniform mat4 u_ViewProj;


void main()
{
    gl_Position = u_ViewProj * vec4(a_Position, 1);
    v_texCoord = a_TexCoord;
    v_Color = a_Color;
    v_FontID = a_FontID;
    v_UseMSDF = a_UseMSDF;
}


#type fragment
#version 440 core
in vec4 v_Color;
in vec2 v_texCoord;
in flat float v_FontID;
in flat int v_UseMSDF;

out vec4 color;

uniform sampler2D u_Fonts[32];

float median(float r, float g, float b) {
    return max(min(r, g), min(max(r, g), b));
}

void main() 
{

    if(v_UseMSDF == 1)
    {
        vec3 msd;
        vec2 size;

        switch(int(v_FontID))
        {
            case 0: msd = texture2D(u_Fonts[0], v_texCoord).rgb; size = textureSize(u_Fonts[0], 0).xy; break;
            case 1: msd = texture2D(u_Fonts[1], v_texCoord).rgb; size = textureSize(u_Fonts[1], 0).xy; break; 
            case 2: msd = texture2D(u_Fonts[2], v_texCoord).rgb; size = textureSize(u_Fonts[2], 0).xy; break;
            case 3: msd = texture2D(u_Fonts[3], v_texCoord).rgb; size = textureSize(u_Fonts[3], 0).xy; break;
            case 4: msd = texture2D(u_Fonts[4], v_texCoord).rgb; size = textureSize(u_Fonts[4], 0).xy; break;
            case 5: msd = texture2D(u_Fonts[5], v_texCoord).rgb; size = textureSize(u_Fonts[5], 0).xy; break;
            case 6: msd = texture2D(u_Fonts[6], v_texCoord).rgb; size = textureSize(u_Fonts[6], 0).xy; break;
            case 7: msd = texture2D(u_Fonts[7], v_texCoord).rgb; size = textureSize(u_Fonts[7], 0).xy; break;
            case 8: msd = texture2D(u_Fonts[8], v_texCoord).rgb; size = textureSize(u_Fonts[8], 0).xy; break;
            case 9: msd = texture2D(u_Fonts[9], v_texCoord).rgb; size = textureSize(u_Fonts[9], 0).xy; break;
            case 10: msd = texture2D(u_Fonts[10], v_texCoord).rgb; size = textureSize(u_Fonts[10], 0).xy; break;
            case 11: msd = texture2D(u_Fonts[11], v_texCoord).rgb; size = textureSize(u_Fonts[11], 0).xy; break;
            case 12: msd = texture2D(u_Fonts[12], v_texCoord).rgb; size = textureSize(u_Fonts[12], 0).xy; break;
            case 13: msd = texture2D(u_Fonts[13], v_texCoord).rgb; size = textureSize(u_Fonts[13], 0).xy; break;
            case 14: msd = texture2D(u_Fonts[14], v_texCoord).rgb; size = textureSize(u_Fonts[14], 0).xy; break;
            case 15: msd = texture2D(u_Fonts[15], v_texCoord).rgb; size = textureSize(u_Fonts[15], 0).xy; break;
            case 16: msd = texture2D(u_Fonts[16], v_texCoord).rgb; size = textureSize(u_Fonts[16], 0).xy; break;
            case 17: msd = texture2D(u_Fonts[17], v_texCoord).rgb; size = textureSize(u_Fonts[17], 0).xy; break;
            case 18: msd = texture2D(u_Fonts[18], v_texCoord).rgb; size = textureSize(u_Fonts[18], 0).xy; break;
            case 19: msd = texture2D(u_Fonts[19], v_texCoord).rgb; size = textureSize(u_Fonts[19], 0).xy; break;
            case 20: msd = texture2D(u_Fonts[20], v_texCoord).rgb; size = textureSize(u_Fonts[20], 0).xy; break;
            case 21: msd = texture2D(u_Fonts[21], v_texCoord).rgb; size = textureSize(u_Fonts[21], 0).xy; break;
            case 22: msd = texture2D(u_Fonts[22], v_texCoord).rgb; size = textureSize(u_Fonts[22], 0).xy; break;
            case 23: msd = texture2D(u_Fonts[23], v_texCoord).rgb; size = textureSize(u_Fonts[23], 0).xy; break;
            case 24: msd = texture2D(u_Fonts[24], v_texCoord).rgb; size = textureSize(u_Fonts[24], 0).xy; break;
            case 25: msd = texture2D(u_Fonts[25], v_texCoord).rgb; size = textureSize(u_Fonts[25], 0).xy; break;
            case 26: msd = texture2D(u_Fonts[26], v_texCoord).rgb; size = textureSize(u_Fonts[26], 0).xy; break;
            case 27: msd = texture2D(u_Fonts[27], v_texCoord).rgb; size = textureSize(u_Fonts[27], 0).xy; break;
            case 28: msd = texture2D(u_Fonts[28], v_texCoord).rgb; size = textureSize(u_Fonts[28], 0).xy; break;
            case 29: msd = texture2D(u_Fonts[29], v_texCoord).rgb; size = textureSize(u_Fonts[29], 0).xy; break;
            case 30: msd = texture2D(u_Fonts[30], v_texCoord).rgb; size = textureSize(u_Fonts[30], 0).xy; break;
            case 31: msd = texture2D(u_Fonts[31], v_texCoord).rgb; size = textureSize(u_Fonts[31], 0).xy; break;
        }


       /* Invert the strength so that 1.0 becomes bold and 0.0 becomes thin */
        float threshold = 0.9;
        float pxRange = 32.0;

        vec2 msdfUnit = pxRange / size;

        float sigDist = median(msd.r, msd.g, msd.b) - threshold;
        sigDist *= dot(msdfUnit, 0.5/ fwidth(v_texCoord));
        float opacity = clamp(sigDist + 0.5, 0.0, 1.0);

        if(opacity == 0)
            discard;

        color = mix(vec4(0.0, 0.0, 0.0, 0.0), v_Color, opacity);

    }
    else
    {
        float opacity;

        switch(int(v_FontID))
        {
            case 0: opacity = texture2D(u_Fonts[0], v_texCoord).a; break;
            case 1: opacity = texture2D(u_Fonts[1], v_texCoord).a; break; 
            case 2: opacity = texture2D(u_Fonts[2], v_texCoord).a; break;
            case 3: opacity = texture2D(u_Fonts[3], v_texCoord).a; break;
            case 4: opacity = texture2D(u_Fonts[4], v_texCoord).a; break;
            case 5: opacity = texture2D(u_Fonts[5], v_texCoord).a; break;
            case 6: opacity = texture2D(u_Fonts[6], v_texCoord).a; break;
            case 7: opacity = texture2D(u_Fonts[7], v_texCoord).a; break;
            case 8: opacity = texture2D(u_Fonts[8], v_texCoord).a; break;
            case 9: opacity = texture2D(u_Fonts[9], v_texCoord).a; break;
            case 10: opacity = texture2D(u_Fonts[10], v_texCoord).a; break;
            case 11: opacity = texture2D(u_Fonts[11], v_texCoord).a; break;
            case 12: opacity = texture2D(u_Fonts[12], v_texCoord).a; break;
            case 13: opacity = texture2D(u_Fonts[13], v_texCoord).a; break;
            case 14: opacity = texture2D(u_Fonts[14], v_texCoord).a; break;
            case 15: opacity = texture2D(u_Fonts[15], v_texCoord).a; break;
            case 16: opacity = texture2D(u_Fonts[16], v_texCoord).a; break;
            case 17: opacity = texture2D(u_Fonts[17], v_texCoord).a; break;
            case 18: opacity = texture2D(u_Fonts[18], v_texCoord).a; break;
            case 19: opacity = texture2D(u_Fonts[19], v_texCoord).a; break;
            case 20: opacity = texture2D(u_Fonts[20], v_texCoord).a; break;
            case 21: opacity = texture2D(u_Fonts[21], v_texCoord).a; break;
            case 22: opacity = texture2D(u_Fonts[22], v_texCoord).a; break;
            case 23: opacity = texture2D(u_Fonts[23], v_texCoord).a; break;
            case 24: opacity = texture2D(u_Fonts[24], v_texCoord).a; break;
            case 25: opacity = texture2D(u_Fonts[25], v_texCoord).a; break;
            case 26: opacity = texture2D(u_Fonts[26], v_texCoord).a; break;
            case 27: opacity = texture2D(u_Fonts[27], v_texCoord).a; break;
            case 28: opacity = texture2D(u_Fonts[28], v_texCoord).a; break;
            case 29: opacity = texture2D(u_Fonts[29], v_texCoord).a; break;
            case 30: opacity = texture2D(u_Fonts[30], v_texCoord).a; break;
            case 31: opacity = texture2D(u_Fonts[31], v_texCoord).a; break;
        }
        if(opacity == 0.0)
            discard;

        color = vec4(v_Color.rgb, opacity);
    }
}