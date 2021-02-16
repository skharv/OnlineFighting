#ifndef _CAMERA_H
#define _CAMERA_H

#include <SFML/Graphics.hpp>

class Camera
{
private:
	sf::Vector2f _position, _size;
	float _zoom = 1;
	
	sf::View _view;
	sf::RectangleShape _viewWindow;
public:
	sf::View GetCamera() { return _view; };
	sf::Vector2f GetStartPoint() { return sf::Vector2f(_view.getCenter().x - _view.getSize().x / 2, _view.getCenter().y - _view.getSize().y / 2); };
	sf::RectangleShape GetViewWindow() { return _viewWindow; };
	void SetZoom(float zoom) { _view.zoom(zoom); };

	void ApplyVector(sf::Vector2f Vector) { _position += Vector; };
	void SetPosition(sf::Vector2f Position) { _position = Position; };
	
	void Update(sf::RenderWindow & Window);

	Camera(sf::Vector2f Size);
	Camera();
	~Camera();
};

#endif