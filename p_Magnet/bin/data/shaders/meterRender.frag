
uniform sampler2DRect alpha;
uniform sampler2DRect render;

void main(){
    // lookup if we can render this
    vec4 texColor       = texture2DRect(alpha, gl_TexCoord[0].st);
    if (texColor.x == 1.0) {
        vec4 color = texture2DRect(render, gl_TexCoord[0].st);
        gl_FragColor = mix(vec4(1.0,1.0,1.0,1.0), color, color.a);
    }
}