xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 26;
 1.01577;1.58248;-0.77937;,
 1.01577;2.47861;-1.51517;,
 1.92294;2.47861;-1.51517;,
 1.92294;1.58248;-0.77937;,
 1.92294;1.90293;-2.21627;,
 1.92294;1.00680;-1.48046;,
 1.01577;1.90293;-2.21627;,
 1.01577;1.00680;-1.48046;,
 1.01577;-2.41026;2.49907;,
 1.92294;-2.41026;2.49907;,
 1.92294;-2.98593;1.79796;,
 1.01577;-2.98593;1.79796;,
 1.84476;2.77781;-2.59504;,
 1.84476;3.05289;-1.54151;,
 1.84476;0.38168;-0.84407;,
 1.84476;0.10661;-1.89760;,
 -1.92294;0.38168;-0.84407;,
 -1.92294;0.10661;-1.89760;,
 -1.92294;3.05289;-1.54151;,
 -1.92294;2.77781;-2.59504;,
 1.84476;3.05289;-1.54151;,
 1.84476;2.77781;-2.59504;,
 1.84476;0.10661;-1.89760;,
 1.84476;2.77781;-2.59504;,
 1.84476;0.38168;-0.84407;,
 1.84476;3.05289;-1.54151;;
 
 16;
 4;0,1,2,3;,
 4;3,2,4,5;,
 4;5,4,6,7;,
 4;8,9,10,11;,
 4;2,1,6,4;,
 4;10,5,7,11;,
 4;9,3,5,10;,
 4;8,0,3,9;,
 4;11,7,0,8;,
 4;7,6,1,0;,
 4;12,13,14,15;,
 4;15,14,16,17;,
 4;17,16,18,19;,
 4;19,18,20,21;,
 4;22,17,19,23;,
 4;16,24,25,18;;
 
 MeshMaterialList {
  1;
  16;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "tyari.jpg";
   }
  }
 }
 MeshNormals {
  12;
  0.000000;0.772857;-0.634580;,
  0.000000;0.634587;0.772852;,
  0.000000;-0.634581;-0.772856;,
  -1.000000;0.000000;0.000000;,
  0.000000;-0.634585;-0.772853;,
  1.000000;0.000000;0.000000;,
  0.000000;-0.967564;0.252625;,
  0.000000;-0.772847;0.634592;,
  0.000000;-0.634588;-0.772851;,
  0.000000;0.967562;-0.252633;,
  0.000000;-0.252627;-0.967564;,
  0.000000;0.252626;0.967564;;
  16;
  4;4,2,2,4;,
  4;3,3,3,3;,
  4;1,1,1,1;,
  4;0,0,0,0;,
  4;7,7,7,7;,
  4;1,1,1,1;,
  4;3,3,3,3;,
  4;8,4,4,8;,
  4;5,5,5,5;,
  4;5,5,5,5;,
  4;5,5,5,5;,
  4;6,6,6,6;,
  4;3,3,3,3;,
  4;9,9,9,9;,
  4;10,10,10,10;,
  4;11,11,11,11;;
 }
 MeshTextureCoords {
  26;
  0.153206;0.218744;,
  0.159914;0.241724;,
  0.159914;0.241724;,
  0.153206;0.218744;,
  0.149467;0.250755;,
  0.142759;0.227776;,
  0.149467;0.250755;,
  0.142759;0.227776;,
  0.123319;0.116359;,
  0.123319;0.116359;,
  0.112872;0.125390;,
  0.112872;0.125390;,
  0.464778;0.300139;,
  0.495715;0.300139;,
  0.495715;0.269202;,
  0.464778;0.269202;,
  0.495715;0.238265;,
  0.464778;0.238265;,
  0.495715;0.207327;,
  0.464778;0.207327;,
  0.495715;0.176390;,
  0.464778;0.176390;,
  0.433840;0.238265;,
  0.433840;0.207327;,
  0.526652;0.238265;,
  0.526652;0.207327;;
 }
}
