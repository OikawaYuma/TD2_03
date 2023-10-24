#pragma once

#include "Mesh.h"
#include "TextureManager.h"
#include "Transform.h"

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "vertexData.h"
#include "ModelData.h"
#include "MaterialData.h"
#include "mathFunction.h"
#include "Material.h"
#include "TransformationMatrix.h"

#include<Windows.h>
#include<d3d12.h>
#include<dxgi1_6.h>
#include <dxcapi.h>
#include <fstream>
#include <sstream>
#include <cassert>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"dxcompiler.lib")
class DirectXCommon;
class WinApp;
class TextureManager;
class Camera;
class Model
{
public:
	ModelData modelData_;
	TransformationMatrix* wvpData;
	ModelData GetModelData() { return modelData_; }
	Model();
	~Model();
	void Initialize(const std::string& directoryPath, const std::string& filename,Camera* camera);
	void Update();
	void Draw(Transform transform);
	void Release();

	void SetTextureManager(TextureManager* textureManager) {
		textureManager_ = textureManager;
	}
	ModelData LoadObjFile(const std::string& directoryPath, const std::string& filename);
	MaterialData LoadMaterialTemplateFile(const std::string& directoryPath,const std::string& filename);
private:
	HRESULT hr;
	// RootSignature作成
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	// シリアライズしてバイナリにする
	ID3DBlob* signatureBlob;
	ID3DBlob* errorBlob;
	// バイナリを元に生成
	ID3D12RootSignature* rootSignature;
	// InputLayout
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[3] = {};
	D3D12_INPUT_LAYOUT_DESC  inputLayoutDesc{};
	// blendStateの設定
	D3D12_BLEND_DESC blendDesc{};
	// RasiterzerStateの設定
	D3D12_RASTERIZER_DESC rasterizerDesc{};

	TextureManager* textureManager_ = nullptr;

	IDxcBlob* vertexShaderBlob;
	IDxcBlob* pixelShaderBlob;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};
	//実際に生成
	ID3D12PipelineState* graphicsPipelineState;
	// RootParmeter作成。複数でっていできるので配列。今回は結果１つだけなので長さ1の配列
	D3D12_ROOT_PARAMETER rootParamerters[4] = {};


	D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};



	D3D12_DESCRIPTOR_RANGE descriptorRange_[1] = {};



	DirectXCommon* directXCommon_;
	WinApp* sWinApp_;

	
	ID3D12Resource* vertexResource_;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};

	VertexData* vertexData_;

	/*色用*/
//頂点リソースの設定
// 実際に頂点リソースを作る
	ID3D12Resource* materialResource;
	// 頂点バッファビューを作成する
	D3D12_VERTEX_BUFFER_VIEW materialBufferView{};
	// 頂点リソースにデータを書き込む
	Material* materialData;

	/*移動用*/
	// WVP用のリソースを作る。Matrix4x4 1つ分のサイズを用意する
	ID3D12Resource* wvpResource;
	// データを書き込む
	
	// 平行光源用
	ID3D12Resource* directionalLightResource;
	// データを書き込む
	DirectionalLight* directionalLightData;
	// 頂点バッファビューを作成する
	D3D12_VERTEX_BUFFER_VIEW wvpBufferView{};
	//ビューポート
	D3D12_VIEWPORT viewport{};
	// シザー矩形
	D3D12_RECT scissorRect{};
	Camera* camera_ = nullptr;
};
