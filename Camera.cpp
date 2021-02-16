#include "Camera.h"

void Camera::Update(sf::RenderWindow &Window)
{
	sf::Vector2f size = _view.getSize();

	_view.setCenter(sf::Vector2f(_position));

	Window.setView(_view);
}

Camera::Camera(sf::Vector2f Size)
{
	_size = Size;
	_position = sf::Vector2f(0, 0);
	_view.reset(sf::FloatRect(0, 0, _size.x, _size.y));
	_view.setCenter(sf::Vector2f(_position));
}

Camera::Camera()
{
}

Camera::~Camera()
{
}
