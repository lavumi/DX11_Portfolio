#pragma once

class TextBuffer;
class TextManager {
public:
	static TextManager* Get();
	static void Delete();


	void Initizlize();

	UINT AddText(D3DXVECTOR2 position, char* text);
	void ChangeText(UINT index, char* text);
	//void MoveText(UINT index, D3DXVECTOR2);

	void Render();


private:
	static TextManager* instance;


	TextManager();
	~TextManager();
	UINT LetterCount = 95;

	struct FontType { 
		float left, right; 
		int size; 
	};
	FontType* m_Font;


	struct TextBox {
		TextBox();
		~TextBox();

		void Initialize(int maxLength, TextManager* manager);
		void SetText(char* sentence);
		void Render();
			
		VertexTexture* vtxData;
		TextManager* manager;
		ID3D11Buffer* vertexBuffer;
		D3DXVECTOR2 position;
		int vertexCount, maxLength;
		float r, g, b;
	};

	vector<TextBox*> textData;









	void LoadFontData();

	

	ID3D11ShaderResourceView* fontImage;
	ID3D11Buffer* vertexBuffer;

	WorldBuffer* wBuffer;

	

	int m_screenWidth, m_screenHeight;

	wstring datafile;
};