
// Screen size as uniform input variables
uniform float screenWidth;
uniform float screenHeight;

// sets the fragment color based on where the pixel is in screen space

void main()
{
	// red color increases to the right 
	// green color increases upward 
	gl_FragColor = vec4(gl_FragCoord.x/screenWidth, gl_FragCoord.y/screenHeight, 0, 1.0);
}
