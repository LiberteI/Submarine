void generateSurfaceMesh();

extern GLfloat oceanSurfaceUnitLength;
// subdivide the surface into stripe - 1 pieces
extern GLint oceanSurfaceVertexCount;
extern std::vector<std::array<GLfloat, 3>> oceanSurfaceVertices;
extern std::vector<GLint> oceanSurfaceIndices;

extern GLdouble oceanTop;
extern GLdouble oceanDepth;

extern GLuint VAO;
extern GLuint VBO;
extern GLuint EBO;