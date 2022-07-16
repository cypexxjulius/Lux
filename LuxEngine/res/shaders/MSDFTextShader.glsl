#type vertex
#version 440 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in float a_FontID;

out vec4 v_Color;
out vec2 v_texCoord;
out flat float v_FontID;

uniform mat4 u_ViewProj;

void main()
{ 
    v_Color = a_Color;
    v_texCoord = a_TexCoord;
    v_FontID = a_FontID;
    gl_Position = u_ViewProj * vec4(a_Position, 1.0);
}


#type fragment
#version 440 core
in vec4 v_Color;
in vec2 v_texCoord;
in flat float v_FontID;

out vec4 color;

uniform sampler2D u_Fonts[32];

float median(float r, float g, float b) {
    return max(min(r, g), min(max(r, g), b));
}

float screenPxRange()
{
    return 2.0;
}

void main() {

    vec3 msd;
    switch(int(v_FontID))
    {
        case 0: msd = texture2D(u_Fonts[0], v_texCoord).rgb; break;
        case 1: msd = texture2D(u_Fonts[1], v_texCoord).rgb; break; 
        case 2: msd = texture2D(u_Fonts[2], v_texCoord).rgb; break;
        case 3: msd = texture2D(u_Fonts[3], v_texCoord).rgb; break;
        case 4: msd = texture2D(u_Fonts[4], v_texCoord).rgb; break;
        case 5: msd = texture2D(u_Fonts[5], v_texCoord).rgb; break;
        case 6: msd = texture2D(u_Fonts[6], v_texCoord).rgb; break;
        case 7: msd = texture2D(u_Fonts[7], v_texCoord).rgb; break;
        case 8: msd = texture2D(u_Fonts[8], v_texCoord).rgb; break;
        case 9: msd = texture2D(u_Fonts[9], v_texCoord).rgb; break;
        case 10: msd = texture2D(u_Fonts[10], v_texCoord).rgb; break;
        case 11: msd = texture2D(u_Fonts[11], v_texCoord).rgb; break;
        case 12: msd = texture2D(u_Fonts[12], v_texCoord).rgb; break;
        case 13: msd = texture2D(u_Fonts[13], v_texCoord).rgb; break;
        case 14: msd = texture2D(u_Fonts[14], v_texCoord).rgb; break;
        case 15: msd = texture2D(u_Fonts[15], v_texCoord).rgb; break;
        case 16: msd = texture2D(u_Fonts[16], v_texCoord).rgb; break;
        case 17: msd = texture2D(u_Fonts[17], v_texCoord).rgb; break;
        case 18: msd = texture2D(u_Fonts[18], v_texCoord).rgb; break;
        case 19: msd = texture2D(u_Fonts[19], v_texCoord).rgb; break;
        case 20: msd = texture2D(u_Fonts[20], v_texCoord).rgb; break;
        case 21: msd = texture2D(u_Fonts[21], v_texCoord).rgb; break;
        case 22: msd = texture2D(u_Fonts[22], v_texCoord).rgb; break;
        case 23: msd = texture2D(u_Fonts[23], v_texCoord).rgb; break;
        case 24: msd = texture2D(u_Fonts[24], v_texCoord).rgb; break;
        case 25: msd = texture2D(u_Fonts[25], v_texCoord).rgb; break;
        case 26: msd = texture2D(u_Fonts[26], v_texCoord).rgb; break;
        case 27: msd = texture2D(u_Fonts[27], v_texCoord).rgb; break;
        case 28: msd = texture2D(u_Fonts[28], v_texCoord).rgb; break;
        case 29: msd = texture2D(u_Fonts[29], v_texCoord).rgb; break;
        case 30: msd = texture2D(u_Fonts[30], v_texCoord).rgb; break;
        case 31: msd = texture2D(u_Fonts[31], v_texCoord).rgb; break;
    }

    float sd = median(msd.r, msd.g, msd.b);
    float screenPxDistance = screenPxRange()*(sd - 0.5);
    float opacity = clamp(screenPxDistance + 0.5, 0.0, 1.0);
    color = mix(vec4(1.0, 0.0, 0.0, 0.0), v_Color, opacity);
}