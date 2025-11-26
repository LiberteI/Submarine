void myDisplay();

void myUpdate();

void sychronizeCameraWithSubmarine();

void computeCurCamPos();

GLfloat computeHeightAtVertex();

extern std::array<GLfloat, 3> cameraCurrentPos;

extern GLfloat horizontalAngle;
extern GLfloat verticalAngle;

extern GLfloat cameraOffset;

extern GLint waveAmplitude;

extern GLfloat curTime;
extern GLfloat waveSpeed;
extern GLfloat phase;