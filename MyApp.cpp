#include "MyApp.h"
#include "ObjParser.h"
#include "SDL_GLDebugMessageCallback.h"

#include <imgui.h>

CMyApp::CMyApp()
{
}

CMyApp::~CMyApp()
{
}

void CMyApp::SetupDebugCallback()
{
	// if the program is running in the debug context, allow and setup the debug callback function
	GLint context_flags;
	glGetIntegerv(GL_CONTEXT_FLAGS, &context_flags);
	if (context_flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
		glDebugMessageControl(GL_DONT_CARE, GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR, GL_DONT_CARE, 0, nullptr, GL_FALSE);
		glDebugMessageCallback(SDL_GLDebugMessageCallback, nullptr);
	}
}

void CMyApp::InitShaders()
{
	m_programID = glCreateProgram();
	AttachShader(m_programID, GL_VERTEX_SHADER, "Shaders/Vert_PosNormTex.vert");
	AttachShader(m_programID, GL_FRAGMENT_SHADER, "Shaders/Frag_LightingRobot.frag");
	LinkProgram(m_programID);
}

void CMyApp::CleanShaders()
{
	glDeleteProgram(m_programID);
}

MeshObject<Vertex> createCube()
{
	MeshObject<Vertex> mesh;

	mesh.vertexArray = {
		// Front face
		{{-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}}, // Bottom-left
		{{0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},  // Bottom-right
		{{0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},   // Top-right
		{{-0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},  // Top-left

		// Back face
		{{0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}}, // Bottom-left
		{{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f}},// Bottom-right
		{{-0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}}, // Top-right
		{{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f}},  // Top-left

		// Left face
		{{-0.5f, -0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},// Bottom-left
		{{-0.5f, -0.5f, 0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}}, // Bottom-right
		{{-0.5f, 0.5f, 0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},  // Top-right
		{{-0.5f, 0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}}, // Top-left

		// Right face
		{{0.5f, -0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},  // Bottom-left
		{{0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}}, // Bottom-right
		{{0.5f, 0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},  // Top-right
		{{0.5f, 0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},   // Top-left

		// Top face
		{{-0.5f, 0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},  // Bottom-left
		{{0.5f, 0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},   // Bottom-right
		{{0.5f, 0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},  // Top-right
		{{-0.5f, 0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}}, // Top-left

		// Bottom face
		{{-0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f}},// Bottom-left
		{{0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.0f}}, // Bottom-right
		{{0.5f, -0.5f, 0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f}},  // Top-right
		{{-0.5f, -0.5f, 0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f}}, // Top-left
	};

	mesh.indexArray = {// Front face
					   0, 1, 2, 2, 3, 0,
					   // Back face
					   4, 5, 6, 6, 7, 4,
					   // Left face
					   8, 9, 10, 10, 11, 8,
					   // Right face
					   12, 13, 14, 14, 15, 12,
					   // Top face
					   16, 17, 18, 18, 19, 16,
					   // Bottom face
					   20, 21, 22, 22, 23, 20 };

	return mesh;
}

void CMyApp::InitGeometry()
{
	const std::initializer_list<VertexAttributeDescriptor> vertexAttribList =
	{
		{0, offsetof(Vertex, position), 3, GL_FLOAT},
		{1, offsetof(Vertex, normal), 3, GL_FLOAT},
		{2, offsetof(Vertex, texcoord), 2, GL_FLOAT},
	};

	m_cubeGPU = CreateGLObjectFromMesh(createCube(), vertexAttribList);

	// reused meshobject
	MeshObject<Vertex> mesh;

	mesh = ObjParser::parse("Assets/head.obj");
	m_robotHeadGPU = CreateGLObjectFromMesh(mesh, vertexAttribList);

	mesh = ObjParser::parse("Assets/torso.obj");
	m_robotTorsoGPU = CreateGLObjectFromMesh(mesh, vertexAttribList);

	mesh = ObjParser::parse("Assets/right_upper_arm.obj");
	m_robotRightArmUpperGPU = CreateGLObjectFromMesh(mesh, vertexAttribList);

	mesh = ObjParser::parse("Assets/right_lower_arm.obj");
	m_robotRightArmLowerGPU = CreateGLObjectFromMesh(mesh, vertexAttribList);

	mesh = ObjParser::parse("Assets/left_upper_arm.obj");
	m_robotLeftArmUpperGPU = CreateGLObjectFromMesh(mesh, vertexAttribList);

	mesh = ObjParser::parse("Assets/left_lower_arm.obj");
	m_robotLeftArmLowerGPU = CreateGLObjectFromMesh(mesh, vertexAttribList);

	mesh = ObjParser::parse("Assets/leg.obj");
	m_robotLegGPU = CreateGLObjectFromMesh(mesh, vertexAttribList);
}

void CMyApp::CleanGeometry()
{
	CleanOGLObject(m_cubeGPU);
	CleanOGLObject(m_robotHeadGPU);
	CleanOGLObject(m_robotTorsoGPU);
	CleanOGLObject(m_robotRightArmUpperGPU);
	CleanOGLObject(m_robotRightArmLowerGPU);
	CleanOGLObject(m_robotLeftArmUpperGPU);
	CleanOGLObject(m_robotLeftArmLowerGPU);
	CleanOGLObject(m_robotLegGPU);
}

void CMyApp::InitTextures()
{
	glCreateSamplers(1, &m_SamplerID);
	glSamplerParameteri(m_SamplerID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glSamplerParameteri(m_SamplerID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glSamplerParameteri(m_SamplerID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glSamplerParameteri(m_SamplerID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	ImageRGBA woodImage = ImageFromFile("Assets/Wood_Table_Texture.png");

	glCreateTextures(GL_TEXTURE_2D, 1, &m_woodTextureID);
	glTextureStorage2D(m_woodTextureID, NumberOfMIPLevels(woodImage), GL_RGBA8, woodImage.width, woodImage.height);
	glTextureSubImage2D(m_woodTextureID, 0, 0, 0, woodImage.width, woodImage.height, GL_RGBA, GL_UNSIGNED_BYTE, woodImage.data());

	glGenerateTextureMipmap(m_woodTextureID);

	ImageRGBA robotImage = ImageFromFile("Assets/Robot_Texture.png");

	glCreateTextures(GL_TEXTURE_2D, 1, &m_robotTextureID);
	glTextureStorage2D(m_robotTextureID, NumberOfMIPLevels(robotImage), GL_RGBA8, robotImage.width, robotImage.height);
	glTextureSubImage2D(m_robotTextureID, 0, 0, 0, robotImage.width, robotImage.height, GL_RGBA, GL_UNSIGNED_BYTE, robotImage.data());

	glGenerateTextureMipmap(m_robotTextureID);

	ImageRGBA shineImage = ImageFromFile("Assets/shine.png");

	glCreateTextures(GL_TEXTURE_2D, 1, &m_shineTextureID);
	glTextureStorage2D(m_shineTextureID, NumberOfMIPLevels(shineImage), GL_RGBA8, shineImage.width, shineImage.height);
	glTextureSubImage2D(m_shineTextureID, 0, 0, 0, shineImage.width, shineImage.height, GL_RGBA, GL_UNSIGNED_BYTE, shineImage.data());

	glGenerateTextureMipmap(m_shineTextureID);
}

void CMyApp::CleanTextures()
{
	glDeleteTextures(1, &m_woodTextureID);
	glDeleteTextures(1, &m_robotTextureID);
	glDeleteTextures(1, &m_shineTextureID);

	glDeleteSamplers( 1, &m_SamplerID );
}

bool CMyApp::Init()
{
	SetupDebugCallback();

	// set the screen clearing color to blue-ish
	glClearColor(0.125f, 0.25f, 0.5f, 1.0f);

	InitShaders();
	InitGeometry();
	InitTextures();

	//
	// other initializations
	//

	glEnable(GL_CULL_FACE); // enable the culling of backwards facing polygons (from the point of view of the camera)
	glCullFace(GL_BACK);    // GL_BACK: backwards facing polygons, GL_FRONT: forwards facing polygons

	glEnable(GL_DEPTH_TEST); // enabling depth testing (obstruction)

	// camera
	m_camera.SetView(
		glm::vec3(0.0, 60.0, 150.0), // the camera's position  - eye
		glm::vec3(0.0, 0.0, 0.0),    // the position that the camera is looking at  - at
		glm::vec3(0.0, 1.0, 0.0));   // the world's up direction  - up

	m_cameraManipulator.SetCamera(&m_camera);

	return true;
}

void CMyApp::Clean()
{
	CleanShaders();
	CleanGeometry();
	CleanTextures();
}

Ray CMyApp::CalculatePixelRay(glm::vec2 pixel) const
{
	// Calculating NDC coordinates
	glm::vec3 pickedNDC = glm::vec3(
		2.0f * (pixel.x + 0.5f) / m_windowSize.x - 1.0f,
		1.0f - 2.0f * (pixel.y + 0.5f) / m_windowSize.y, 0.0f);

	// Calculating the world coordinates with the help of the inverse ViewProj matrix
	glm::vec4 pickedWorld = glm::inverse(m_camera.GetViewProj()) * glm::vec4(pickedNDC, 1.0f);
	pickedWorld /= pickedWorld.w; // homogeneous division
	Ray ray;

	// Raycasting starting position is the camera's position
	ray.origin = m_camera.GetEye();
	// Direction is from the camera's position towards the clicked point's world coordinates
	// ATTENTION: NOT normalized vector!
	ray.direction = glm::vec3(pickedWorld) - ray.origin;
	return ray;
}

static bool HitPlane(const Ray& ray, const glm::vec3& planeQ, const glm::vec3& planeI, const glm::vec3& planeJ, Intersection& result)
{
	// planeQ + u * planeI + v * planeJ
	glm::mat3 A(-ray.direction, planeI, planeJ);
	glm::vec3 B = ray.origin - planeQ;

	if (fabsf(glm::determinant(A)) < 1e-6) return false;
	glm::vec3 X = glm::inverse(A) * B;

	if (X.x < 0.0) {
		return false;
	}
	result.t = X.x;
	result.uv.x = X.y;
	result.uv.y = X.z;

	return true;
}

bool CMyApp::intersectTableTop(const Ray& ray, const glm::vec3& tablePos, const glm::vec3& tableSize, glm::vec3& result)
{
	// plane
	glm::vec3 planeQ = tablePos + 0.5f * glm::vec3(-tableSize.x, tableSize.y, -tableSize.z);
	glm::vec3 planeI = glm::vec3(tableSize.x, 0.0f, 0.0f);
	glm::vec3 planeJ = glm::vec3(0.0f, 0.0f, tableSize.z);
	Intersection inter;
	// if there is an intersection, and it is within the square
	if (HitPlane(ray, planeQ, planeI, planeJ, inter)
		&& (inter.uv.x >= 0 && inter.uv.x <= 1)
		&& (inter.uv.y >= 0 && inter.uv.y <= 1)
		) {
		const glm::vec2& uv = inter.uv;
		result = planeQ + planeI * uv.x + planeJ * uv.y;
		return true;
	}
	else {
		return false;
	}
}

void CMyApp::Update(const SUpdateInfo& updateInfo)
{
	m_ElapsedTimeInSec = updateInfo.ElapsedTimeInSec;

	if (m_IsPicking) {
		// starting a ray through the clicked pixel
		Ray ray = CalculatePixelRay(glm::vec2(m_PickedPixel.x, m_PickedPixel.y));
		// intersecting with the table
		glm::vec3 desiredPos;
		if (intersectTableTop(ray, TABLE_POS, TABLE_SIZE, desiredPos)) {
			CalculateRobotMovementData(desiredPos);
		}

        m_IsPicking = false;
	}

	CalculateRobotPosition(updateInfo.DeltaTimeInSec);

	if ( m_isTopCamera )
	{
        glm::vec3 camPosInObjSpace = glm::vec3( 0.0f, 60.0f, -150.0f );
        glm::mat4 matCam = glm::translate( m_robotState.position ) * glm::rotate( glm::radians( m_robotState.rotation ), glm::vec3( 0.0f, 1.0f, 0.0f ) );
        glm::vec3 camPos = glm::vec3( matCam * glm::vec4(camPosInObjSpace,1.0f ) ); 

		m_camera.SetView(
			camPos,  // the camera's position  - eye
			m_robotState.position,  // the position that the camera is looking at  - at
			glm::vec3( 0.0f, 1.0f, 0.0f ) ); // the world's up direction  - up
	}
	else
	{
		m_cameraManipulator.Update(updateInfo.DeltaTimeInSec);
	}

    // Robot transformations

    m_robotTransform = 
		glm::translate( m_robotState.position ) * 
		glm::rotate( glm::radians( m_robotState.rotation ), glm::vec3( 0, 1, 0 ) );

	m_robotBaseTransform = 
		m_robotTransform * 
		glm::translate(TORSO_POS);
	m_robotHeadTransform = 
		m_robotBaseTransform * 
		glm::translate(RELATIVE_HEAD_POS) *
		glm::rotate(m_robotHeadLeftRightRot, glm::vec3(0, -1, 0)) *
		glm::rotate(m_robotHeadUpDownRot, glm::vec3(1, 0, 0));
	m_robotLeftLegTransform =
		m_robotBaseTransform *
		glm::translate(RELATIVE_LEFT_LEG_POS) *
		glm::translate(CalculateLegAnim(m_robotState.legTime, 0.0f));
	m_robotRightLegTransform =
		m_robotBaseTransform *
		glm::translate(RELATIVE_RIGHT_LEG_POS) *
		glm::translate(CalculateLegAnim(m_robotState.legTime, glm::pi<float>()));
	m_robotLeftArmUpperTransform = 
		m_robotBaseTransform * 
		glm::translate(RELATIVE_LEFT_ARM_UPPER) * 
		glm::rotate(m_robotLeftArmUpperRot, glm::vec3(-1, 0, 0));
	m_robotRightArmUpperTransform = 
		m_robotBaseTransform * 
		glm::translate(RELATIVE_RIGHT_ARM_UPPER) * 
		glm::rotate(m_robotRightArmUpperRot, glm::vec3(-1, 0, 0));
	m_robotLeftArmLowerTransform = 
		m_robotLeftArmUpperTransform * // !!
		glm::translate(RELATIVE_LEFT_ARM_LOWER) * 
		glm::rotate(m_robotLeftArmLowerRot, glm::vec3(-1, 0, 0));
	m_robotRightArmLowerTransform = 
		m_robotRightArmUpperTransform * // !!
		glm::translate(RELATIVE_RIGHT_ARM_LOWER) * 
		glm::rotate(m_robotRightArmLowerRot, glm::vec3(-1, 0, 0));

	// intersection of the table and the ray starting from the robot
	Ray headRay = CalculateRobotHeadRay();
	glm::vec3 intersectionPoint;
	if (intersectTableTop(headRay, TABLE_POS, TABLE_SIZE, intersectionPoint)) {
		m_lightPosition2 = glm::vec4(intersectionPoint + glm::vec3(0, 1.0f, 0), 1.0f);
	}
	else {
		m_lightPosition2 = glm::vec4(0.0f);
	}
}

void CMyApp::SetCommonUniforms()
{
	// - Uniform parameters

	// - view and projection matrices
	glUniformMatrix4fv( ul("viewProj"), 1, GL_FALSE, glm::value_ptr(m_camera.GetViewProj()));

	// - Setting up the light sources
	glUniform3fv( ul("cameraPosition"), 1, glm::value_ptr(m_camera.GetEye()));
	glUniform4fv( ul("lightPosition"), 1, glm::value_ptr(m_lightPosition));

	glUniform3fv( ul("La"), 1, glm::value_ptr(m_La));
	glUniform3fv( ul("Ld"), 1, glm::value_ptr(m_Ld));
	glUniform3fv( ul("Ls"), 1, glm::value_ptr(m_Ls));

	if ( m_lightPosition2.w != 0.0f )
	{
		glUniform4fv( ul("lightPosition2"), 1, glm::value_ptr( m_lightPosition2 ) );
		glUniform3fv( ul("La2"), 1, glm::value_ptr( m_La2 ) );
		glUniform3fv( ul("Ld2"), 1, glm::value_ptr( m_Ld2 ) );
		glUniform3fv( ul("Ls2"), 1, glm::value_ptr( m_Ls2 ) );
	}
	else
	{
		glUniform3fv( ul("La2"), 1, glm::value_ptr( glm::vec3( 0, 0, 0 ) ) );
		glUniform3fv( ul("Ld2"), 1, glm::value_ptr( glm::vec3( 0, 0, 0 ) ) );
		glUniform3fv( ul("Ls2"), 1, glm::value_ptr( glm::vec3( 0, 0, 0 ) ) );
	}

	glUniform1f( ul("lightConstantAttenuation"), m_lightConstantAttenuation);
	glUniform1f( ul("lightLinearAttenuation"), m_lightLinearAttenuation);
	glUniform1f( ul("lightQuadraticAttenuation"), m_lightQuadraticAttenuation);
}

void CMyApp::DrawObject(OGLObject& obj, const glm::mat4& world) {
	glUniformMatrix4fv( ul("world"), 1, GL_FALSE, glm::value_ptr(world));
	glUniformMatrix4fv( ul("worldInvTransp"), 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(world))));
	glBindVertexArray(obj.vaoID);
	glDrawElements(GL_TRIANGLES, obj.count, GL_UNSIGNED_INT, nullptr);
}

bool is_rotation_positive(float angle1, float angle2)
{
    glm::vec3 v1 = glm::vec3( cosf( glm::radians(angle1) ), sinf( glm::radians(angle1) ), 0.0f );
	glm::vec3 v2 = glm::vec3( cosf( glm::radians(angle2) ), sinf( glm::radians(angle2) ), 0.0f );

    return glm::cross( v1, v2 ).z >= 0.0f;
}



void CMyApp::CalculateRobotPosition(float deltaTime)
{
	if (m_robotState.state == RobotState::ROBOT_STATE_STAND) { return; }

	m_robotState.legTime += deltaTime;

	if (m_robotState.state == RobotState::ROBOT_STATE_ROTATING)
	{
		// rotation

        float& rotation = m_robotState.rotation;

        bool isRotationPositive = is_rotation_positive( rotation, m_robotState.desiredRotation );
        float desiredRotation = m_robotState.desiredRotation;

        if ( !isRotationPositive && desiredRotation > rotation ) desiredRotation -= 360.0f;
		if ( isRotationPositive && desiredRotation < rotation ) desiredRotation += 360.0f;

		bool rotationPhaseEnded = false;

		if ( isRotationPositive )
		{
            rotation += m_robotState.rotationSpeed * deltaTime;
			if ( rotation > desiredRotation ) rotationPhaseEnded = true;
        }
        else
        {
            rotation -= m_robotState.rotationSpeed * deltaTime;
            if ( rotation < desiredRotation ) rotationPhaseEnded = true;
        }

		if ( rotationPhaseEnded )
		{
			rotation = m_robotState.desiredRotation;
			m_robotState.state = RobotState::ROBOT_STATE_MOVING;
		}
	}

	if (m_robotState.state == RobotState::ROBOT_STATE_MOVING)
	{
		// movement
		
		glm::vec3& pos = m_robotState.position;
		const glm::vec3& desiredPosition = m_robotState.desiredPosition;

        glm::vec3 desiredDiff = desiredPosition - pos;
		glm::vec3 movingDiffPerSec = glm::normalize(desiredDiff)*m_robotState.movementSpeed;
		
        glm::vec3 movingVector = movingDiffPerSec * deltaTime;

        pos += movingVector;

		if ( glm::length( desiredDiff ) <= glm::length( movingVector ) )
		{
            pos = desiredPosition;
            m_robotState.state = RobotState::ROBOT_STATE_STAND;
		}
	}
}

glm::vec3 CMyApp::CalculateLegAnim(float time, float animOffset)
{
	float legAnim = -((time * (glm::two_pi<float>()) * LEG_SPEED) + animOffset);
	return glm::vec3(0.0, glm::clamp(sinf(legAnim), 0.0f, 1.0f), cosf(legAnim) * LEG_SPREAD);
}

Ray CMyApp::CalculateRobotHeadRay() const
{
	Ray headRay;
    headRay.origin = glm::vec3( m_robotHeadTransform * glm::vec4( AVG_EYE_POS_OFFSET, 1 ) );
    headRay.direction = glm::vec3( m_robotHeadTransform * glm::vec4( 0, 0, 1, 0 ) );

	return headRay;
}

void CMyApp::RenderRobot()
{
	// Texture
	glBindTextureUnit(0, m_robotTextureID);
	glBindTextureUnit(1, m_shineTextureID);
	glBindSampler(0, m_SamplerID);
	glBindSampler(1, m_SamplerID);
	glUniform1i( ul("textureImage"), 0);
	glUniform1i( ul("textureShinning"), 1);
	// - Setting up the material attributes
	glUniform3fv( ul("Ka"), 1, glm::value_ptr(glm::vec3(1, 1, 1)));
	glUniform3fv( ul("Kd"), 1, glm::value_ptr(glm::vec3(1, 1, 1)));
	glUniform3fv( ul("Ks"), 1, glm::value_ptr(glm::vec3(1, 1, 1)));
	// state - drawing mode
	glUniform1i( ul("state"), SHADER_STATE_ROBOT);

	DrawObject( m_robotTorsoGPU, m_robotBaseTransform );

	DrawObject( m_robotHeadGPU, m_robotHeadTransform );
    DrawObject( m_robotLegGPU, m_robotLeftLegTransform );
    DrawObject( m_robotLegGPU, m_robotRightLegTransform );
	DrawObject( m_robotLeftArmUpperGPU, m_robotLeftArmUpperTransform );
    DrawObject( m_robotRightArmUpperGPU, m_robotRightArmUpperTransform );
	DrawObject( m_robotLeftArmLowerGPU, m_robotLeftArmLowerTransform );
	DrawObject( m_robotRightArmLowerGPU, m_robotRightArmLowerTransform );
}

void CMyApp::RenderTable()
{
	// - Setting up the material attributes
	glUniform3fv( ul("Ka"), 1, glm::value_ptr(m_Ka));
	glUniform3fv( ul("Kd"), 1, glm::value_ptr(m_Kd));
	glUniform3fv( ul("Ks"), 1, glm::value_ptr(m_Ks));

	glUniform1f( ul("Shininess"), m_Shininess);

	// - setting up the texture uniform variables
	glUniform1i( ul("textureImage"), 0);

	// Binding the textures, separately for each one
	glBindTextureUnit(0, m_woodTextureID);
	glBindSampler(0, m_SamplerID);
	
    // Setting up the table's state
	glUniform1i( ul("state"), SHADER_STATE_TABLE);

	glm::mat4 tableTransform = glm::translate(TABLE_POS);

	// Start the drawing process
	DrawObject(m_cubeGPU, tableTransform * glm::scale(TABLE_SIZE));

	glUniform1i( ul("state"), SHADER_STATE_DEFAULT);
	
	for (int i = -1; i <= 1; i += 2) {
		for (int j = -1; j <= 1; j += 2) {
			glm::vec3 legPos = glm::vec3(i * 60, -(TABLE_LEG_SIZE.y + TABLE_SIZE.y) / 2.f, j * 60);
			DrawObject(m_cubeGPU, tableTransform * glm::translate(legPos) * glm::scale(TABLE_LEG_SIZE));
		}
	}
}

void CMyApp::Render()
{
	// clear the framebuffer (GL_COLOR_BUFFER_BIT)
	// ... and the depth buffer (GL_DEPTH_BUFFER_BIT)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(m_programID);

	SetCommonUniforms();

	glUniform1i( ul("debugState"), m_shaderDebugState);

	RenderRobot();
	RenderTable();

	// debug cube, the robot's destination
	if (m_drawRobotGoal) {
		glUniform1i( ul("state"), SHADER_STATE_DEFAULT);
		glm::mat4 goalWorldTr = glm::translate(m_robotState.desiredPosition) * glm::scale(glm::vec3(m_robotCubeGoalSize));
		DrawObject(m_cubeGPU, goalWorldTr);
	}


	glBindVertexArray(0);
	// turn the shader off
	glUseProgram(0);
}

void CMyApp::CalculateRobotMovementData(glm::vec3 desiredPosition)
{
	m_robotState.desiredPosition = desiredPosition;
	glm::vec3 diff = desiredPosition - m_robotState.position;
	// from -pi to pi
	float atan = glm::atan(diff.x, diff.z);
	m_robotState.desiredRotation = glm::mod(glm::degrees(atan) + 360.f, 360.0f);

	m_robotState.state = RobotState::ROBOT_STATE_ROTATING;
}

void CMyApp::RenderGUI()
{
	//ImGui::ShowDemoWindow();

	if (ImGui::Begin("Robot")) {
		ImGui::SeparatorText("Animations");
		ImGui::SliderAngle("Left Upper Arm", &m_robotLeftArmUpperRot, -90.0f, 180.0f);
		ImGui::SliderAngle("Left Lower Arm", &m_robotLeftArmLowerRot, -120.0f, 120.0f);
		ImGui::SliderAngle("Right Upper Arm", &m_robotRightArmUpperRot, -90.0f, 180.0f);
		ImGui::SliderAngle("Right Lower Arm", &m_robotRightArmLowerRot, -120.0f, 120.0f);
		ImGui::SliderAngle("Head Up-Down", &m_robotHeadUpDownRot, -20.0f, 50.0f);
		ImGui::SliderAngle("Head Left-Right", &m_robotHeadLeftRightRot, -120.0f, 120.0f);

		ImGui::SeparatorText("Shader Debug");
		ImGui::Combo("Debug State", &m_shaderDebugState, "None\0Normals\0Shading\0Diffuse texture\0Shine texture\0");

		ImGui::SeparatorText("3rd Prerson View");
		ImGui::Checkbox("3rd Person View Camera", &m_isTopCamera);

		ImGui::SeparatorText("Robot goal draw");
		ImGui::Checkbox("Draw robot goal", &m_drawRobotGoal);
		ImGui::SliderFloat("Goal cube size", &m_robotCubeGoalSize, 1.0f, 100.f);

		ImGui::SeparatorText("Robot moving speeds");
        ImGui::SliderFloat( "Movement speed", &m_robotState.movementSpeed, 1.0f, 100.0f );
        ImGui::SliderFloat( "Rotation speed", &m_robotState.rotationSpeed, 1.0f, 360.0f );

	}
	ImGui::End();
}

// https://wiki.libsdl.org/SDL2/SDL_KeyboardEvent
// https://wiki.libsdl.org/SDL2/SDL_Keysym
// https://wiki.libsdl.org/SDL2/SDL_Keycode
// https://wiki.libsdl.org/SDL2/SDL_Keymod

void CMyApp::KeyboardDown(const SDL_KeyboardEvent& key)
{
	if (!key.repeat) // It was pressed for the first time
	{
		if (key.key == SDLK_F5 && key.mod & SDL_KMOD_CTRL)
		{
			CleanShaders();
			InitShaders();
		}
		if (key.key == SDLK_F1)
		{
			GLint polygonModeFrontAndBack[2] = {};
			// https://registry.khronos.org/OpenGL-Refpages/gl4/html/glGet.xhtml
			glGetIntegerv( GL_POLYGON_MODE, polygonModeFrontAndBack ); // Query the current polygon mode! It gives separate modes for front and back.
			GLenum polygonMode = ( polygonModeFrontAndBack[ 0 ] != GL_FILL ? GL_FILL : GL_LINE ); // Let's toggle between FILL and LINE!
			// https://registry.khronos.org/OpenGL-Refpages/gl4/html/glPolygonMode.xhtml
			glPolygonMode( GL_FRONT_AND_BACK, polygonMode ); // Let's set the new one!
		}

		if (key.key == SDLK_LCTRL || key.key == SDLK_RCTRL)
		{
			m_IsCtrlDown = true;
		}
	}
	m_cameraManipulator.KeyboardDown(key);
}

void CMyApp::KeyboardUp(const SDL_KeyboardEvent& key)
{
	m_cameraManipulator.KeyboardUp(key);
	if (key.key == SDLK_LCTRL || key.key == SDLK_RCTRL)
	{
		m_IsCtrlDown = false;
	}
}

// https://wiki.libsdl.org/SDL2/SDL_MouseMotionEvent

void CMyApp::MouseMove(const SDL_MouseMotionEvent& mouse)
{
	m_cameraManipulator.MouseMove(mouse);
}

// https://wiki.libsdl.org/SDL2/SDL_MouseButtonEvent

void CMyApp::MouseDown(const SDL_MouseButtonEvent& mouse)
{
	if ( m_IsCtrlDown )
	{
		m_IsPicking = true;
	}
	m_PickedPixel = { mouse.x, mouse.y };
}

void CMyApp::MouseUp(const SDL_MouseButtonEvent& mouse)
{
}

// https://wiki.libsdl.org/SDL2/SDL_MouseWheelEvent

void CMyApp::MouseWheel(const SDL_MouseWheelEvent& wheel)
{
	m_cameraManipulator.MouseWheel(wheel);
}

// two new parameters for the resized window's width (_w) and height (_h)
void CMyApp::Resize(int _w, int _h)
{
	glViewport(0, 0, _w, _h);
	m_windowSize = glm::uvec2(_w, _h);
	m_camera.SetAspect(static_cast<float>(_w) / _h);
}

// For handling other, more exotic events that haven't been handled yet
// https://wiki.libsdl.org/SDL2/SDL_Event

void CMyApp::OtherEvent(const SDL_Event& ev)
{

}