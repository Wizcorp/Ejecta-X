varying highp vec2 vUv;
varying lowp vec4 vColor;

uniform mediump vec4 tint;

uniform sampler2D texture;

void main() {
    lowp vec4 tint2 = vec4(0.0, 1.0, 0.0, 1.0);
	gl_FragColor = texture2D(texture, vUv) * vColor * tint2;
}
