#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform sampler2D texture;

varying vec2 v_texcoord;

void main()
{
    // Set fragment color from texture
    vec4 texColor = texture2D(texture, v_texcoord);

    // Use alpha channel information for transparency
    if (texColor.r > 0.99 &&texColor.g > 0.99 && texColor.b > 0.99)
        discard;

    gl_FragColor = texColor;
}
