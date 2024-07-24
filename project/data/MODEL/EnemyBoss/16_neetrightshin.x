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
 63;
 -1.90314;-20.84959;0.95799;,
 0.05560;-20.86489;1.83676;,
 0.04668;-17.93157;2.88427;,
 -2.09049;-17.79026;1.72399;,
 2.23071;-20.83507;0.95701;,
 2.18234;-17.77599;1.72302;,
 2.68758;-20.77873;-1.16584;,
 3.61037;-16.91172;-1.11824;,
 1.91446;-20.72718;-3.28835;,
 2.14079;-16.86103;-3.28460;,
 0.05373;-20.71083;-4.16719;,
 0.04144;-16.84216;-4.29234;,
 -1.80646;-20.73960;-3.28751;,
 -2.05729;-16.87504;-3.28365;,
 -2.60366;-20.79699;-1.16460;,
 -3.52552;-16.93555;-1.11662;,
 -1.90314;-20.84959;0.95799;,
 -2.09049;-17.79026;1.72399;,
 0.03845;-15.43608;3.30900;,
 -2.45318;-15.41198;2.04638;,
 2.52929;-15.39535;2.04525;,
 3.84933;-15.31269;-1.00462;,
 2.26841;-15.25456;-3.47526;,
 0.03604;-15.23805;-4.40842;,
 -2.19575;-15.26946;-3.47424;,
 -3.77513;-15.33814;-1.00290;,
 -2.45318;-15.41198;2.04638;,
 0.02081;-10.45720;5.13433;,
 -3.72891;-10.42384;3.34759;,
 3.76941;-10.39881;3.34589;,
 4.75316;-10.28478;-0.96995;,
 3.21866;-10.20546;-4.26013;,
 0.02238;-11.64041;-5.34139;,
 -3.18291;-10.22683;-4.25868;,
 -4.71536;-10.31639;-0.96781;,
 -3.72891;-10.42384;3.34759;,
 0.00291;-3.79823;3.26874;,
 -3.37561;-3.78040;2.13525;,
 3.38072;-3.75785;2.13372;,
 4.18786;-3.68488;-0.60479;,
 3.61996;-1.31478;-3.74142;,
 -0.00688;-1.29547;-4.96478;,
 -3.63296;-1.33899;-3.73978;,
 -4.18446;-3.71284;-0.60289;,
 -3.37561;-3.78040;2.13525;,
 0.05560;-20.86489;1.83676;,
 -1.90314;-20.84959;0.95799;,
 0.05466;-20.78786;-1.16522;,
 2.23071;-20.83507;0.95701;,
 2.68758;-20.77873;-1.16584;,
 1.91446;-20.72718;-3.28835;,
 0.05373;-20.71083;-4.16719;,
 -1.80646;-20.73960;-3.28751;,
 -2.60366;-20.79699;-1.16460;,
 -3.37561;-3.78040;2.13525;,
 0.00291;-3.79823;3.26874;,
 -0.00423;-0.02023;0.04687;,
 3.38072;-3.75785;2.13372;,
 4.18786;-3.68488;-0.60479;,
 3.61996;-1.31478;-3.74142;,
 -0.00688;-1.29547;-4.96478;,
 -3.63296;-1.33899;-3.73978;,
 -4.18446;-3.71284;-0.60289;;
 
 48;
 4;0,1,2,3;,
 4;1,4,5,2;,
 4;4,6,7,5;,
 4;6,8,9,7;,
 4;8,10,11,9;,
 4;10,12,13,11;,
 4;12,14,15,13;,
 4;14,16,17,15;,
 4;3,2,18,19;,
 4;2,5,20,18;,
 4;5,7,21,20;,
 4;7,9,22,21;,
 4;9,11,23,22;,
 4;11,13,24,23;,
 4;13,15,25,24;,
 4;15,17,26,25;,
 4;19,18,27,28;,
 4;18,20,29,27;,
 4;20,21,30,29;,
 4;21,22,31,30;,
 4;22,23,32,31;,
 4;23,24,33,32;,
 4;24,25,34,33;,
 4;25,26,35,34;,
 4;28,27,36,37;,
 4;27,29,38,36;,
 4;29,30,39,38;,
 4;30,31,40,39;,
 4;31,32,41,40;,
 4;32,33,42,41;,
 4;33,34,43,42;,
 4;34,35,44,43;,
 3;45,46,47;,
 3;48,45,47;,
 3;49,48,47;,
 3;50,49,47;,
 3;51,50,47;,
 3;52,51,47;,
 3;53,52,47;,
 3;46,53,47;,
 3;54,55,56;,
 3;55,57,56;,
 3;57,58,56;,
 3;58,59,56;,
 3;59,60,56;,
 3;60,61,56;,
 3;61,62,56;,
 3;62,54,56;;
 
 MeshMaterialList {
  6;
  48;
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2;;
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\Documentsneet.png";
   }
  }
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\neett.jpg";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\neett.png";
   }
  }
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\UV_Grid.png";
   }
  }
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\UV_Grid.png";
   }
  }
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  53;
  -0.725250;-0.240309;0.645185;,
  -0.007355;-0.293627;0.955892;,
  0.743123;-0.210575;0.635158;,
  0.987301;-0.135442;-0.083017;,
  0.693445;-0.078183;-0.716255;,
  -0.000125;-0.030484;-0.999535;,
  -0.691490;-0.081013;-0.717829;,
  -0.984487;-0.164877;-0.060011;,
  -0.723643;-0.211996;0.656809;,
  -0.003298;-0.241477;0.970401;,
  0.733095;-0.194625;0.651684;,
  0.985287;-0.147758;-0.085884;,
  0.668390;-0.094432;-0.737792;,
  0.000016;-0.072523;-0.997367;,
  -0.667207;-0.097995;-0.738398;,
  -0.983338;-0.166057;-0.073966;,
  -0.724987;-0.241797;0.644925;,
  0.001136;-0.272560;0.962138;,
  0.726914;-0.236757;0.644626;,
  0.976143;-0.196406;-0.092571;,
  0.650034;-0.165709;-0.741617;,
  0.000368;-0.179023;-0.983845;,
  -0.649231;-0.170242;-0.741294;,
  -0.974795;-0.203193;-0.092133;,
  -0.754906;-0.074314;0.651610;,
  0.000446;-0.059054;0.998255;,
  0.755672;-0.069588;0.651243;,
  0.994537;-0.082879;-0.063467;,
  0.686168;-0.095138;-0.721195;,
  0.000122;-0.100715;-0.994915;,
  -0.685904;-0.099504;-0.720857;,
  -0.994033;-0.089092;-0.062943;,
  -0.641994;0.469670;0.606015;,
  -0.001279;0.457738;0.889086;,
  0.639565;0.473004;0.605990;,
  0.895968;0.417758;0.150729;,
  0.706501;0.040584;-0.706548;,
  -0.000323;0.039255;-0.999229;,
  -0.707135;0.036734;-0.706123;,
  -0.898068;0.413011;0.151312;,
  0.003434;-0.999664;-0.025697;,
  -0.002676;0.938608;0.344974;,
  0.003515;-0.999665;-0.025653;,
  0.003565;-0.999661;-0.025778;,
  0.003395;-0.999664;-0.025720;,
  0.003460;-0.999665;-0.025663;,
  0.003396;-0.999667;-0.025596;,
  0.003332;-0.999665;-0.025652;,
  0.003379;-0.999664;-0.025694;,
  0.003428;-0.999661;-0.025821;,
  0.406900;0.910114;0.078266;,
  -0.003258;0.969113;-0.246594;,
  -0.412187;0.907678;0.078884;;
  48;
  4;0,1,9,8;,
  4;1,2,10,9;,
  4;2,3,11,10;,
  4;3,4,12,11;,
  4;4,5,13,12;,
  4;5,6,14,13;,
  4;6,7,15,14;,
  4;7,0,8,15;,
  4;8,9,17,16;,
  4;9,10,18,17;,
  4;10,11,19,18;,
  4;11,12,20,19;,
  4;12,13,21,20;,
  4;13,14,22,21;,
  4;14,15,23,22;,
  4;15,8,16,23;,
  4;16,17,25,24;,
  4;17,18,26,25;,
  4;18,19,27,26;,
  4;19,20,28,27;,
  4;20,21,29,28;,
  4;21,22,30,29;,
  4;22,23,31,30;,
  4;23,16,24,31;,
  4;24,25,33,32;,
  4;25,26,34,33;,
  4;26,27,35,34;,
  4;27,28,36,35;,
  4;28,29,37,36;,
  4;29,30,38,37;,
  4;30,31,39,38;,
  4;31,24,32,39;,
  3;42,43,40;,
  3;44,42,40;,
  3;45,44,40;,
  3;46,45,40;,
  3;47,46,40;,
  3;48,47,40;,
  3;49,48,40;,
  3;43,49,40;,
  3;32,33,41;,
  3;33,34,41;,
  3;34,35,41;,
  3;35,50,41;,
  3;50,51,41;,
  3;51,52,41;,
  3;52,39,41;,
  3;39,32,41;;
 }
 MeshTextureCoords {
  63;
  0.421450;0.192278;,
  0.429531;0.192278;,
  0.429531;0.167974;,
  0.421450;0.167974;,
  0.437612;0.192278;,
  0.437612;0.167974;,
  0.445693;0.192278;,
  0.445693;0.167974;,
  0.453774;0.192278;,
  0.453774;0.167974;,
  0.461856;0.192278;,
  0.461856;0.167974;,
  0.469937;0.192278;,
  0.469937;0.167974;,
  0.478018;0.192278;,
  0.478018;0.167974;,
  0.486099;0.192278;,
  0.486099;0.167974;,
  0.429531;0.143670;,
  0.421450;0.143670;,
  0.437612;0.143670;,
  0.445693;0.143670;,
  0.453774;0.143670;,
  0.461856;0.143670;,
  0.469937;0.143670;,
  0.478018;0.143670;,
  0.486099;0.143670;,
  0.429531;0.119366;,
  0.421450;0.119366;,
  0.437612;0.119366;,
  0.445693;0.119366;,
  0.453774;0.119366;,
  0.461856;0.119366;,
  0.469937;0.119366;,
  0.478018;0.119366;,
  0.486099;0.119366;,
  0.429531;0.095061;,
  0.421450;0.095061;,
  0.437612;0.095061;,
  0.445693;0.095061;,
  0.453774;0.095061;,
  0.461856;0.095061;,
  0.469937;0.095061;,
  0.478018;0.095061;,
  0.486099;0.095061;,
  0.453774;0.273090;,
  0.482346;0.261255;,
  0.453774;0.234300;,
  0.425203;0.261255;,
  0.413369;0.232684;,
  0.425203;0.204113;,
  0.453774;0.192278;,
  0.482346;0.204113;,
  0.494180;0.232684;,
  0.482346;0.026327;,
  0.453774;0.014493;,
  0.453774;0.056515;,
  0.425203;0.026327;,
  0.413369;0.054899;,
  0.425203;0.083470;,
  0.453774;0.095304;,
  0.482346;0.083470;,
  0.494180;0.054899;;
 }
}
