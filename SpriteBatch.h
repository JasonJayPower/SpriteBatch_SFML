#pragma once

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/VertexBuffer.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>

#include <array>
#include <vector>

enum class SpriteSortMode { 
	None, FrontToBack, BackToFront, Texture 
};

class SpriteBatch {
public:
	explicit SpriteBatch(sf::RenderTarget& target);

	void begin(SpriteSortMode sortMode);
	void draw(const sf::Sprite& sprite);
	void end();

private:
	static constexpr auto MaxSprites = 1024;
	static constexpr auto VerticesPerSprite = 4;

	struct SpriteData {
		const sf::Texture* texture{};
		std::array<sf::Vertex, VerticesPerSprite> vertices{};
	};

	void flushBatch();
	void sortSpriteData();
	void renderBatch(sf::Uint64 start, sf::Uint64 end);

	sf::Uint64 m_spriteCount;
	sf::RenderTarget& m_target;
	sf::RenderStates m_states;
	std::vector<std::unique_ptr<SpriteData>> m_spriteData;
	std::vector<sf::Vertex> m_vertices;
	SpriteSortMode m_sortMode;

	//sf::Int32 m_drawCalls;
	//sf::Int32 m_texSwitch;
};
