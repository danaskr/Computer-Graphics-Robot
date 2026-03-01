#include "CameraManipulator.h"

#include "Camera.h"

#include <SDL3/SDL.h>

CameraManipulator::CameraManipulator()
{
}

CameraManipulator::~CameraManipulator()
{
}

void CameraManipulator::SetCamera( Camera* _pCamera )
{
    m_pCamera = _pCamera;

    if ( !m_pCamera ) return;

	// Set the initial spherical coordinates.
    m_center = m_pCamera->GetAt();
    glm::vec3 ToAim = m_center - m_pCamera->GetEye();

    m_distance = glm::length( ToAim );

    m_u = atan2f( ToAim.z, ToAim.x );
    m_v = acosf( ToAim.y / m_distance );

    m_worldUp = m_pCamera->GetWorldUp();
}

void CameraManipulator::Update( float _deltaTime )
{
    if ( !m_pCamera ) return;

	// Updating the camera model according to the parameters.

	// The new look direction is calculated in spherical coordinates.
    glm::vec3 lookDirection( cosf(m_u) * sinf(m_v),
                             cosf(m_v), 
                             sinf(m_u) * sinf(m_v) );
	// The new camera position is calculated from the look direction and the distance.
    glm::vec3 eye = m_center - m_distance * lookDirection;

	// The new up direction should be equal to the world's up direction.
    glm::vec3 up = m_worldUp;

	// The new right direction is calculated by taking the cross product of the look direction and the up direction.
    glm::vec3 right = glm::normalize( glm::cross( lookDirection, up ) );

	// The new forward direction is calculated by taking the cross product of the up direction and the right direction.
    glm::vec3 forward = glm::cross( up, right);

	// The new delta position is calculated by using the camera's movement directions and speed.
    glm::vec3 deltaPosition = ( m_goForward * forward + m_goRight * right + m_goUp * up ) * m_speed * _deltaTime;

	// Setting the new camera position and center.
    eye += deltaPosition;
    m_center += deltaPosition;   

	// Updating the camera with the new position and looking direction.
    m_pCamera->SetView( eye, m_center, m_worldUp );
}


void CameraManipulator::KeyboardDown(const SDL_KeyboardEvent& key)
{
	switch ( key.key )
	{
	case SDLK_LSHIFT:
	case SDLK_RSHIFT:
		if ( !key.repeat ) m_speed /= 4.0f;
		break;
	case SDLK_W:
		m_goForward = 1;
		break;
	case SDLK_S:
		m_goForward = -1;
		break;
	case SDLK_A:
		m_goRight = -1;
		break;
	case SDLK_D:
		m_goRight = 1;
		break;
	case SDLK_E:
		m_goUp = 1;
		break;
	case SDLK_Q:
		m_goUp = -1;
		break;
	}
}

void CameraManipulator::KeyboardUp(const SDL_KeyboardEvent& key)
{
	
	switch ( key.key )
	{
	case SDLK_LSHIFT:
	case SDLK_RSHIFT:
		m_speed *= 4.0f;
		break;
	case SDLK_W:
	case SDLK_S:
		m_goForward = 0;
		break;
	case SDLK_A:
	case SDLK_D:
		m_goRight = 0;
		break;
	case SDLK_Q:
	case SDLK_E:
		m_goUp = 0;
		break;
	}
}


void CameraManipulator::MouseMove(const SDL_MouseMotionEvent& mouse)
{
	if ( mouse.state & SDL_BUTTON_LMASK )
	{
		float du = mouse.xrel / 100.0f;
		float dv = mouse.yrel / 100.0f;

		m_u += du;
		m_v = glm::clamp<float>( m_v + dv, 0.1f, 3.1f );
	}
	if ( mouse.state & SDL_BUTTON_RMASK )
	{
		m_distance *= pow( 0.9f, mouse.yrel / 50.0f );
	}
}

void CameraManipulator::MouseWheel(const SDL_MouseWheelEvent& wheel)
{
	m_distance *= powf( 0.9f, static_cast<float>( wheel.y ) );
}