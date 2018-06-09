cbuffer ConstBuffer : register (b0) {
	matrix world;			// ���[���h�ϊ��s��
	matrix view;			// �r���[�ϊ��s��
	matrix projection;		// �����ˉe�ϊ��s��
	float4 Diffuse;
};

struct VS_IN {
	float4 pos : POSITION;
	float4 nor : NORMAL;
	float4 col : COLOR;
	float2 tex : TEXCOORD;
};

struct VS_OUT {
	float4 pos  : SV_POSITION;
	float4 col  : COLOR;
	float2 tex  : TEXCOORD;
};


VS_OUT vs_main( VS_IN input ) {
	VS_OUT output;

	output.pos = mul(input.pos, world);
	output.pos = mul(output.pos, view);
	output.pos = mul(output.pos, projection);

	output.col = input.col;
	output.tex = input.tex;

	return output;
}