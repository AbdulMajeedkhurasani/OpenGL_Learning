#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;
const float offset = 1.0 / 300.0;

void main()
{
    // orignal
    FragColor = texture(screenTexture, TexCoords);
    
    /*
    //Color inversion
    FragColor = vec4(vec3(1.0 - texture(screenTexture, TexCoords)), 1.0);
    */
    /*
    //Gray scale 
    FragColor = texture(screenTexture, TexCoords);
    // average gray scale
    //float average = (FragColor.r + FragColor.g + FragColor.b) / 3.0;
    // weighted gray scale 
    float average = 0.2126 * FragColor.r + 0.7152 * FragColor.g + 0.0722 * FragColor.b;
    FragColor = vec4(average, average, average, 1.0);
    */

    /*
    // kernel or convolution 
    // if convolution matrix weights don't add up to 1 the resulting 
    // picture will end up darker or brighter
    // assuming convolution matrix is 3x3

    vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // top-left
        vec2( 0.0,    offset), // top-center
        vec2( offset,  offset), // top-right
        vec2(-offset,  0.0),   // center-left
        vec2( 0.0,    0.0),   // center-center
        vec2( offset,  0.0),   // center-right
        vec2(-offset, -offset), // bottom-left
        vec2( 0.0,   -offset), // bottom-center
        vec2( offset, -offset)  // bottom-right
    );

    float filterSharp[9] = float[](
        -1, -1, -1,
        -1,  9, -1,
        -1, -1, -1);

    float filterEdge[9] = float[](
        -1, -1, -1,
        -1,  8, -1,
        -1, -1, -1);

        // /16 to made weights add up to 1
    float filterBlur[9] = float[](
        1.0 / 16, 2.0 / 16, 1.0 / 16,
        2.0 / 16, 4.0 / 16, 2.0 / 16,
        1.0 / 16, 2.0 / 16, 1.0 / 16);

    vec3 color = vec3(0.0);
    for(int i = 0; i < 9; i++)
    {
        color += filterEdge[i] *
         vec3(texture(screenTexture, TexCoords.st + offsets[i]));
    }

    FragColor = vec4(color, 1.0);
    */
} 