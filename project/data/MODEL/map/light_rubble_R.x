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
 18;
 -11.09561;-4.20377;6.22294;,
 -10.81924;23.69663;4.59655;,
 -10.36787;18.13507;-20.41254;,
 -10.94185;-12.58122;9.83551;,
 -10.20880;20.23998;-30.30473;,
 -6.61357;20.46434;-30.67048;,
 -6.85049;-12.56911;9.70551;,
 -5.85371;29.61323;10.13209;,
 -10.71394;29.42618;10.56839;,
 11.22946;-3.85669;7.17351;,
 11.05531;19.32802;-21.52687;,
 13.46414;23.38690;4.44638;,
 4.88779;-12.49904;9.74425;,
 5.46453;20.56693;-30.66783;,
 9.98424;20.79129;-31.03360;,
 9.06898;-12.56911;9.70551;,
 12.99767;29.61323;10.13209;,
 5.29584;29.74818;10.16102;;
 
 18;
 3;0,1,2;,
 4;3,4,5,6;,
 3;5,7,6;,
 3;3,6,7;,
 3;7,5,4;,
 3;4,3,7;,
 3;4,8,7;,
 3;7,8,3;,
 3;3,8,4;,
 3;9,10,11;,
 4;12,13,14,15;,
 3;14,16,15;,
 3;12,15,16;,
 3;16,14,13;,
 3;13,12,16;,
 3;13,17,16;,
 3;16,17,12;,
 3;12,17,13;;
 
 MeshMaterialList {
  3;
  18;
  0,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  0,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1;;
  Material {
   0.800000;0.800000;0.800000;0.450000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.120000;0.120000;0.120000;;
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "metal02.jpg";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  24;
  -0.999762;0.008738;-0.019987;,
  -0.016976;-0.773848;-0.633144;,
  0.999635;-0.023487;-0.013350;,
  -0.057193;0.973914;-0.219593;,
  0.060958;-0.014366;0.998037;,
  0.031787;-0.010858;0.999436;,
  -0.069971;0.973415;-0.218101;,
  -0.082737;0.972754;-0.216573;,
  -0.997512;0.062398;0.032804;,
  -0.989766;0.118798;0.079061;,
  0.090077;-0.017861;0.995775;,
  -0.999891;0.005663;-0.013630;,
  0.992938;-0.089268;-0.078138;,
  -0.015859;-0.773584;-0.633496;,
  0.994338;-0.092072;-0.053057;,
  0.016269;0.975522;-0.219300;,
  0.006331;-0.010428;0.999926;,
  0.009080;-0.010957;0.999899;,
  -0.024405;0.976942;-0.212105;,
  -0.065037;0.976694;-0.204548;,
  -0.992912;0.098540;0.066457;,
  -0.972750;0.186056;0.138352;,
  0.003583;-0.009899;0.999945;,
  -0.999933;0.009720;-0.006317;;
  18;
  3;0,0,0;,
  4;1,1,1,1;,
  3;2,2,2;,
  3;4,5,4;,
  3;6,7,6;,
  3;8,8,9;,
  3;6,3,6;,
  3;4,10,4;,
  3;8,11,8;,
  3;12,12,12;,
  4;13,13,13,13;,
  3;14,14,14;,
  3;16,17,16;,
  3;18,19,18;,
  3;20,20,21;,
  3;18,15,18;,
  3;16,22,16;,
  3;20,23,20;;
 }
 MeshTextureCoords {
  18;
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.140320;1.000000;,
  0.140320;1.000000;,
  1.000000;1.000000;,
  0.140320;0.814250;,
  0.000000;0.000000;,
  0.000000;1.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  1.000000;1.000000;,
  0.140320;1.000000;,
  0.140320;1.000000;,
  1.000000;1.000000;,
  0.140320;0.814250;,
  0.000000;0.000000;;
 }
}
