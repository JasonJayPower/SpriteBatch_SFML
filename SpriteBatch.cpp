#include "SpriteBatch.h"
//#include <iostream>

SpriteBatch::SpriteBatch(sf::RenderTarget& target)
	: m_spriteCount { 0 }
	, m_target      { target }
    , m_spriteData  { std::vector<std::unique_ptr<SpriteData>>(MaxSprites) }
	, m_vertices    { std::vector<sf::Vertex>(MaxSprites * VerticesPerSprite, sf::Vertex()) }
	, m_sortMode    { SpriteSortMode::None }
	//, m_drawCalls   { 0 }
	//, m_texSwitch   { 0 }
{
	for (sf::Uint64 i = 0; i < MaxSprites; ++i) {
		m_spriteData[i] = std::make_unique<SpriteData>();
	}
}

void SpriteBatch::begin(SpriteSortMode sortMode)
{
	m_sortMode = sortMode;
}

void SpriteBatch::draw(const sf::Sprite& sprite)
{
	if (m_spriteCount >= MaxSprites) {
		flushBatch();
		//++m_drawCalls;
	}

	sf::Uint32 vertexCount = 0;
	SpriteData* const spriteData = m_spriteData[m_spriteCount].get();
	spriteData->texture = sprite.getTexture();

	const sf::FloatRect& texRect = static_cast<sf::FloatRect>(sprite.getTextureRect());
	const sf::Transform& transform = sprite.getTransform();

	spriteData->vertices[vertexCount] = transform.transformPoint(0.0f, 0.0f);
	spriteData->vertices[vertexCount++].texCoords = sf::Vector2f(texRect.left, texRect.top);

	spriteData->vertices[vertexCount].position = transform.transformPoint(0.0f, texRect.height);
	spriteData->vertices[vertexCount++].texCoords = sf::Vector2f(texRect.left, texRect.top + texRect.height);

	spriteData->vertices[vertexCount].position = transform.transformPoint(texRect.width, texRect.height);
	spriteData->vertices[vertexCount++].texCoords = sf::Vector2f(texRect.left + texRect.width, texRect.top + texRect.height);

	spriteData->vertices[vertexCount].position = transform.transformPoint(texRect.width, 0.0f);
	spriteData->vertices[vertexCount++].texCoords = sf::Vector2f(texRect.left + texRect.width, texRect.top);

	m_spriteCount++;
}

void SpriteBatch::end()
{
	flushBatch();
	//std::cout << "No. Draw Calls [" << m_drawCalls 
	//	<< " ] No. of Texture Switches [" << m_texSwitch << " ]"  << std::endl;
	//m_drawCalls = 0;
	//m_texSwitch = 0;
}

void SpriteBatch::flushBatch()
{
	if (!m_spriteCount) {
		return;
	}

	sortSpriteData();

	sf::Uint64 batchStart = 0;
	m_states.texture = nullptr;
	for (sf::Uint64 i = 0; i < m_spriteCount; ++i) {
		const sf::Texture* tex = m_spriteData[i].get()->texture;

		if (tex != m_states.texture) {
			if (i > batchStart) {
				renderBatch(batchStart, i); 
				//++m_texSwitch;
				//++m_drawCalls;
			}
			m_states.texture = tex;
			batchStart = i;
		}
	}
	renderBatch(batchStart, m_spriteCount);
	m_spriteCount = 0;
}


void SpriteBatch::sortSpriteData()
{
	switch (m_sortMode) {
		case SpriteSortMode::Texture: {
			std::sort(m_spriteData.begin(), m_spriteData.begin() + m_spriteCount,
				[](const auto& s1, const auto& s2) noexcept -> bool {
					return (s1->texture < s2->texture);
			});
		} break;
		case SpriteSortMode::BackToFront: {
			std::sort(m_spriteData.begin(), m_spriteData.begin() + m_spriteCount,
				[](const auto& s1, const auto& s2) noexcept -> bool {
					return (s1->vertices[0].position.y < s2->vertices[0].position.y);
				});
		} break;
		case SpriteSortMode::FrontToBack: {
			std::sort(m_spriteData.begin(), m_spriteData.begin() + m_spriteCount,
				[](const auto& s1, const auto& s2) noexcept -> bool {
					return (s1->vertices[0].position.y > s2->vertices[0].position.y);
				});
		} break;
		default: {
		} break;
	}
}

void SpriteBatch::renderBatch(sf::Uint64 start, sf::Uint64 end)
{
	for (sf::Uint64 i = start; i < end; i++) {
		const SpriteData* const spriteData = m_spriteData[i].get();
		for (sf::Uint64 j = 0; j < VerticesPerSprite; j++) {
			m_vertices[(i * VerticesPerSprite) + j] = spriteData->vertices[j];
		}
	}
	m_target.draw(&m_vertices[start * VerticesPerSprite], (end - start) * VerticesPerSprite, sf::PrimitiveType::Quads, m_states);
}
