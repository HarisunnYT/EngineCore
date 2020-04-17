#include "..\Header Files\MeshRenderer.h"

#include "Drawable.h"
#include "Texture.h"

MeshRenderer::MeshRenderer(Drawable* draw) : MeshRenderer(draw, nullptr)
{
}

MeshRenderer::MeshRenderer(Drawable* draw, Texture* text)
{
	drawable = draw;
	texture = text;

	drawable->meshRenderer = this;
}

void MeshRenderer::Draw()
{
	drawable->Draw();
}

Texture* MeshRenderer::GetTexture()
{
	return texture;
}
