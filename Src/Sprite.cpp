#include "Sprite.h"
#include "WinApp.h"
#include "DirectXCommon.h"
#include "Camera.h"
#include "mathFunction.h"
#include "Mesh.h"



Sprite::Sprite() {};
void Sprite::Initialize() {
	sWinApp = WinApp::GetInstance();
	sDirectXCommon = DirectXCommon::GetInstance();
	
	rootParamerters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE; // DescripterTableを使う
	rootParamerters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで使う
	rootParamerters[0].DescriptorTable.pDescriptorRanges = descriptorRange_; // Tableの中身の配列を指定
	rootParamerters[0].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange_); // Tableで利用する数

	// Sprite用の頂点リソースを作る
	vertexResourceSprite_ = CreateBufferResourceA(sDirectXCommon->GetDevice(), sizeof(VertexData) * 4);

	// リソースを先頭のアドレスから使う
	vertexBufferViewSprite_.BufferLocation = vertexResourceSprite_->GetGPUVirtualAddress();
	// 使用するリソースのサイズは頂点6つ分のサイズ
	vertexBufferViewSprite_.SizeInBytes = sizeof(VertexData) * 4;
	// 1頂点あたりのサイズ
	vertexBufferViewSprite_.StrideInBytes = sizeof(VertexData);

	vertexResourceSprite_->Map(0, nullptr, reinterpret_cast<void**>(&vertexDataSprite_));

	// 1枚目の三角形
	vertexDataSprite_[0].position = { 0.0f,360.0f,0.0f,1.0f };//左下
	vertexDataSprite_[0].texcorrd = {0.0f,1.0f};
	vertexDataSprite_[1].position = {0.0f,0.0f,0.0f,1.0f}; // 左上
	vertexDataSprite_[1].texcorrd = {0.0f,0.0f};
	vertexDataSprite_[2].position = {640.0f,360.0f,0.0f,1.0f}; // 右下
	vertexDataSprite_[2].texcorrd = {1.0f,1.0f};
	
	vertexDataSprite_[3].position = {640.0f,0.0f,0.0f,1.0f}; // 右上
	vertexDataSprite_[3].texcorrd = {1.0f,0.0f};
	
	// 実際に頂点リソースを作る
	materialResource = CreateBufferResourceA(sDirectXCommon->GetDevice(), sizeof(Vector4));

	materialBufferView = CreateBufferView();;
	// 頂点リソースにデータを書き込む
	materialData = nullptr;
	// 書き込むためのアドレスを取得
	materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));
	// 色のデータを変数から読み込み
	materialData->color = {1.0f,1.0f,1.0f,1.0f};
	materialData->uvTransform = MakeIdentity4x4();

	uvTransformSprite = {
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f}
	};

	Matrix4x4 uvTransformmatrix = MakeScaleMatrix(uvTransformSprite.scale);
	uvTransformmatrix = Multiply(uvTransformmatrix, MakeRotateZMatrix(uvTransformSprite.rotate.z));
	uvTransformmatrix = Multiply(uvTransformmatrix, MakeTranslateMatrix(uvTransformSprite.translate));
	materialData->uvTransform = uvTransformmatrix;

	// Sprite用のTransformationMatrix用のリソースを作る。Matrix4x4 1つ分のサイズを用意する
	transformationMatrixResouceSprite = CreateBufferResourceA(sDirectXCommon->GetDevice(),sizeof(Matrix4x4));
	// 書き込むためのアドレスを取得
	transformationMatrixResouceSprite->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixDataSprite));
	// 単位行列を書き込んでおく
	*transformationMatrixDataSprite = MakeIdentity4x4();
	
	// Transform変数の初期化
	transform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };

	indexResourceSprite = CreateBufferResourceA(sDirectXCommon->GetDevice(), sizeof(uint32_t) * 6);
	// リソースの先頭のアドレスから使う
	indexBufferViewSprite.BufferLocation = indexResourceSprite->GetGPUVirtualAddress();
	// 使用するリソースのサイズはインデックス6つ分のサイズ
	indexBufferViewSprite.SizeInBytes = sizeof(uint32_t) * 6;
	// インデックスはuint_tとする
	indexBufferViewSprite.Format = DXGI_FORMAT_R32_UINT;

	// インデックスリソースにデータを書き込む
	uint32_t* indexDataSprite = nullptr;
	indexResourceSprite->Map(0, nullptr, reinterpret_cast<void**>(&indexDataSprite));
	indexDataSprite[0] = 0;
	indexDataSprite[1] = 1;
	indexDataSprite[2] = 2;
	indexDataSprite[3] = 1;
	indexDataSprite[4] = 3;
	indexDataSprite[5] = 2;

};
void Sprite::Update() {
	// Sprite用のWorldViewProjectMatrixを作る
Matrix4x4 worldMatrixSprite = MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);
Matrix4x4 viewMatrixSprite = MakeIdentity4x4();
Matrix4x4 projectionMatrixSprite = MakeOrthographicMatrix(0.0f, 0.0f, float(sWinApp->GetKClientWidth()), float(sWinApp->GetKClientHeight()), 0.0f, 100.0f);
Matrix4x4 worldViewProjectionMatrixSprite = Multiply(worldMatrixSprite, Multiply(viewMatrixSprite, projectionMatrixSprite));
*transformationMatrixDataSprite = worldViewProjectionMatrixSprite;
};

void Sprite::Draw() {
	
	
	
	sDirectXCommon->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferViewSprite_); // VBVを設定
	sDirectXCommon->GetCommandList()->IASetIndexBuffer(&indexBufferViewSprite);
	// TransformationmatrixCBufferの場所を設定
	sDirectXCommon->GetCommandList()->SetGraphicsRootConstantBufferView(1, transformationMatrixResouceSprite->GetGPUVirtualAddress());

	// SRV のDescriptorTableの先頭を設定。2はrootParameter[2]である。
	sDirectXCommon->GetCommandList()->SetGraphicsRootDescriptorTable(2, textureManager_->textureSrvHandleGPU_);
	// 描画（DrawCall/ドローコール）
	//sDirectXCommon->GetCommandList()->DrawInstanced(6, 1, 0, 0);
	sDirectXCommon->GetCommandList()->DrawIndexedInstanced(6, 1, 0, 0,0);
}
void Sprite::Release() {

	transformationMatrixResouceSprite->Release();
	vertexResourceSprite_->Release();
	indexResourceSprite->Release();
	
	
};

ID3D12Resource* Sprite::CreateBufferResourceA(ID3D12Device* device, size_t sizeInBytes) {

	//頂点リソース用のヒープの設定
	D3D12_HEAP_PROPERTIES uploadHeapProperties{};
	uploadHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;// Uploadheapを使う
	D3D12_RESOURCE_DESC resourceDesc{};
	// バッファリソース。テクスチャの場合はまた別の設定をする
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resourceDesc.Width = sizeInBytes;// リソースのサイズ。今回はVector4を3頂点分
	//バッファの場合はこれらは1にする決まり
	resourceDesc.Height = 1;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.MipLevels = 1;
	resourceDesc.SampleDesc.Count = 1;
	// バッファに場合はこれにする決まり
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	ID3D12Resource* resource = nullptr;
	HRESULT hr = device->CreateCommittedResource(&uploadHeapProperties, D3D12_HEAP_FLAG_NONE,
		&resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&resource));
	assert(SUCCEEDED(hr));

	return resource;
};

D3D12_VERTEX_BUFFER_VIEW Sprite::CreateBufferView() {
	D3D12_VERTEX_BUFFER_VIEW view{};

	//リソースの先頭のアドレスから使う
	view.BufferLocation = vertexResourceSprite_->GetGPUVirtualAddress();
	// 使用するリソースのサイズは頂点3つ分のサイズ
	view.SizeInBytes = sizeof(VertexData) * 6;
	// 1頂点あたりのサイズ
	view.StrideInBytes = sizeof(VertexData);


	return view;
};