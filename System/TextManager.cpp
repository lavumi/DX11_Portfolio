#include "../stdafx.h"
#include "TextManager.h"


TextManager::TextManager()
{
	m_Font = 0;

	//wBuffer = 0;
	fontImage = 0;

	datafile = L"./Contents/Font/fontdata.txt";

	D3DInfo info;
	D3D::GetInfo(&info);

	m_screenHeight = info.screenHeight;
	m_screenWidth = info.screenWidth;

	HRESULT hr;
	hr = D3DX11CreateShaderResourceViewFromFile(D3D::GetDevice(), L"./Contents/Font/font.dds", nullptr, nullptr, &fontImage, nullptr);
	assert(SUCCEEDED(hr));
	

	LoadFontData();

	wBuffer = new WorldBuffer();


	D3DXMATRIX world;
	D3DXMatrixIdentity(&world);

	wBuffer->SetWorld(world);
}

TextManager::~TextManager()
{
	SAFE_DELETE_ARRAY(m_Font);
	SAFE_DELETE(wBuffer);
	SAFE_RELEASE(fontImage);

}

UINT TextManager::AddText(D3DXVECTOR2 position, char * text)
{
	TextBox* textbox = new TextBox();
	textbox->Initialize(32, this);
	textData.push_back(textbox);
	float drawX = (float)(((m_screenWidth / 2) * -1) + position.x);
	float drawY = (float)((m_screenHeight / 2) - position.y);

	textbox->position = D3DXVECTOR2(drawX, drawY);
	textbox->SetText(text);
	

	return (UINT)textData.size()-1;
}

void TextManager::ChangeText(UINT index, char * text)
{
	textData[index]->SetText(text);
}

void TextManager::Render()
{

	D3D::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	wBuffer->SetBuffer();
	D3D::GetDeviceContext()->PSSetShaderResources(0, 1, &fontImage);
	for each (TextBox* textBox in textData)
	{
		textBox->Render();
	}
}

void TextManager::LoadFontData()
{

	ifstream fin; 
	int i; 
	char temp;


	m_Font = new FontType[95]; 

	fin.open(datafile);
	if (fin.fail())
		assert(0);
	for(i=0; i<95; i++) { 
		fin.get(temp); 
		while(temp != ' ') {
			fin.get(temp); 
		} 
		fin.get(temp); 
		while(temp != ' ') { 
			fin.get(temp); 
		} 
		fin >> m_Font[i].left; 
		fin >> m_Font[i].right; 
		fin >> m_Font[i].size; 
	}  
	fin.close(); 
}

void TextManager::BuildVertexArray(int textBoxNum, char* text, int positionX, int positionY, float r, float g, float b)
{

}

void TextManager::CreateBuffer()
{
}

TextManager::TextBox::TextBox()
{
	vertexBuffer = 0;
	maxLength = 0;
}

TextManager::TextBox::~TextBox()
{
	SAFE_RELEASE(vertexBuffer);
	SAFE_DELETE_ARRAY(vtxData);
}


//vtxbuffer 만들어 놓는 과정
void TextManager::TextBox::Initialize(int maxLength, TextManager* manager)
{
	this->maxLength = maxLength;
	this->manager = manager;
	vertexCount = maxLength * 6;

	//빈 껍데기 일단 만들어놓자
	vtxData = new VertexTexture[vertexCount];
	ZeroMemory(vtxData, sizeof(VertexTexture)*vertexCount);


	D3D11_BUFFER_DESC desc;
	D3D11_SUBRESOURCE_DATA data;

	//1. Vertex Buffer cpu에서 수정할수 있게
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.ByteWidth = sizeof(VertexTexture) * vertexCount;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
	data.pSysMem = vtxData;

	HRESULT hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &vertexBuffer);
	assert(SUCCEEDED(hr));

	//SAFE_DELETE_ARRAY(vtxData);

}

void TextManager::TextBox::SetText(char * sentence)
{
	ZeroMemory(vtxData, sizeof(VertexTexture)*vertexCount);

	float drawX, drawY;

	drawX = position.x;
	drawY = position.y;

	int numLetters, letter;
	numLetters = (int)strlen(sentence);

	numLetters = min(numLetters, maxLength);

	int index = 0;
	for (int i = 0; i<numLetters; i++)
	{
		letter = ((int)sentence[i]) - 32;

		if (letter == 0)
		{
			drawX = drawX + 3.0f;
		}
		else
		{
			vtxData[index].position = D3DXVECTOR3(drawX, drawY, 0.0f);
			vtxData[index++].uv = D3DXVECTOR2(manager->m_Font[letter].left, 0.0f);

			vtxData[index].position = D3DXVECTOR3((drawX + manager->m_Font[letter].size), (drawY - 16), 0.0f);
			vtxData[index++].uv = D3DXVECTOR2(manager->m_Font[letter].right, 1.0f);

			vtxData[index].position = D3DXVECTOR3(drawX, (drawY - 16), 0.0f);
			vtxData[index++].uv = D3DXVECTOR2(manager->m_Font[letter].left, 1.0f);


			vtxData[index].position = D3DXVECTOR3(drawX, drawY, 0.0f);
			vtxData[index++].uv = D3DXVECTOR2(manager->m_Font[letter].left, 0.0f);

			vtxData[index].position = D3DXVECTOR3(drawX + manager->m_Font[letter].size, drawY, 0.0f);
			vtxData[index++].uv = D3DXVECTOR2(manager->m_Font[letter].right, 0.0f);

			vtxData[index].position = D3DXVECTOR3((drawX + manager->m_Font[letter].size), (drawY - 16), 0.0f);
			vtxData[index++].uv = D3DXVECTOR2(manager->m_Font[letter].right, 1.0f);

			drawX = drawX + manager->m_Font[letter].size + 1.0f;
		}
	}



	//vtx버퍼 재설정
	D3D11_MAPPED_SUBRESOURCE subResource;

	HRESULT hr = D3D::GetDeviceContext()->Map
	(
		vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource
	);

	memcpy(subResource.pData, vtxData, sizeof(VertexTexture) * vertexCount);

	D3D::GetDeviceContext()->Unmap(vertexBuffer, 0);
}


void TextManager::TextBox::Render()
{
	UINT stride = sizeof(VertexTexture);
	UINT offset = 0;
	D3D::GetDeviceContext()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

	D3D::GetDeviceContext()->Draw(vertexCount, 0);

}
