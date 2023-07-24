﻿#pragma once
#include<Windows.h>
#include<d3d12.h>
#include<dxgi1_6.h>
#include<cassert>
#include <dxcapi.h>

//#include"DirXCommon.h"
#include "Vector4.h"
#include"Vector3.h"
#include"Matrix4x4.h"

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"dxcompiler.lib")

class DirXCommon;
class WinApp;

class Mesh
{
public:
	Mesh();
	~Mesh();


	ID3D12Resource* CreateBufferResource(ID3D12Device* device,size_t sizeInBytes);
	D3D12_RESOURCE_DESC  CreateBufferResourceDesc(size_t sizeInBytes);
	D3D12_VERTEX_BUFFER_VIEW  CreateBufferView();

	void Initialize(DirXCommon *sDirX, Vector4* vertexDataA,Vector4 DrawColor);
	void Update(DirXCommon* dirX);
	void Release();
	HRESULT hr;




	// RootSignature作成
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	// シリアライズしてバイナリにする
	ID3DBlob* signatureBlob;
	ID3DBlob* errorBlob;
	// バイナリを元に生成
	ID3D12RootSignature* rootSignature;
	// InputLayout
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[1] = {};
	D3D12_INPUT_LAYOUT_DESC  inputLayoutDesc{};
	// blendStateの設定
	D3D12_BLEND_DESC blendDesc{};
	// RasiterzerStateの設定
	D3D12_RASTERIZER_DESC rasterizerDesc{};



	IDxcBlob* vertexShaderBlob;
	IDxcBlob* pixelShaderBlob;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};
	//実際に生成
	ID3D12PipelineState* graphicsPipelineState;

	
	//頂点リソース用のヒープの設定
	D3D12_HEAP_PROPERTIES uploadHeapProperties{};

	/*頂点用*/
	// 実際に頂点リソースを作る
	ID3D12Resource* vertexResource;
	// 頂点バッファビューを作成する
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
	// 頂点リソースにデータを書き込む
	Vector4* vertexData;


	/*色用*/
	//頂点リソースの設定
	// 実際に頂点リソースを作る
	ID3D12Resource* materialResource;
	// 頂点バッファビューを作成する
	D3D12_VERTEX_BUFFER_VIEW materialBufferView{};
	// 頂点リソースにデータを書き込む
	Vector4* materialData;

	/*移動用*/
	// WVP用のリソースを作る。Matrix4x4 1つ分のサイズを用意する
	ID3D12Resource* wvpResource;
	// データを書き込む
	Matrix4x4* wvpData;

	// 頂点バッファビューを作成する
	D3D12_VERTEX_BUFFER_VIEW wvpBufferView{};

	





	//ビューポート
	D3D12_VIEWPORT viewport{};
	// シザー矩形
	D3D12_RECT scissorRect{};

private:
};
