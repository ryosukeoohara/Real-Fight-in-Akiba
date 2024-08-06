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
 72;
 3.16558;-16.02532;-2.51261;,
 0.28271;-16.04944;-3.69174;,
 0.68806;-11.19205;-3.82008;,
 3.36035;-11.16739;-2.61503;,
 -3.38209;-16.02532;-2.51261;,
 -3.19185;-11.16739;-2.61503;,
 -4.43608;-15.97393;0.00000;,
 -4.49738;-11.11391;0.00000;,
 0.68806;-7.02286;-3.91564;,
 3.36035;-6.99820;-2.71059;,
 -2.88860;-6.99820;-2.71059;,
 -4.24184;-6.94276;0.00000;,
 0.26799;-3.71621;-4.08089;,
 2.72233;-3.69100;-2.84862;,
 -2.46030;-3.69100;-2.84862;,
 -3.65524;-3.63273;0.00000;,
 -0.10722;1.33722;-3.36734;,
 1.70530;1.35983;-2.02290;,
 -2.10829;1.35897;-2.51583;,
 -2.91377;-0.30111;0.00000;,
 3.16558;-16.02532;-2.51261;,
 4.21957;-15.97393;0.00000;,
 1.29300;-17.26629;0.00000;,
 0.28271;-17.29587;-1.45965;,
 0.28271;-16.04944;-3.69174;,
 -3.38209;-16.02532;-2.51261;,
 1.70530;1.35983;-2.02290;,
 -0.10722;1.33722;-3.36734;,
 1.37453;2.69965;-0.95080;,
 -0.10722;2.85790;-1.74500;,
 1.09784;2.85761;0.00000;,
 -0.10722;3.53769;0.00000;,
 -1.41109;2.91899;0.00000;,
 -1.78183;2.74084;-1.17740;,
 -2.10829;1.35897;-2.51583;,
 -2.91377;-0.30111;0.00000;,
 4.45730;-11.11391;-0.00000;,
 4.21957;-15.97393;0.00000;,
 4.49738;-6.94276;-0.00000;,
 3.91727;-3.63273;-0.00000;,
 2.69110;0.02675;0.00000;,
 0.28271;-17.29090;0.00000;,
 0.16016;-17.28993;-0.00000;,
 0.37912;-17.28993;0.00000;,
 -0.92134;-17.26629;0.00000;,
 -4.43608;-15.97393;0.00000;,
 2.69110;0.02675;0.00000;,
 3.16558;-16.02532;2.51261;,
 3.36035;-11.16739;2.61503;,
 0.68806;-11.19205;3.82008;,
 0.28271;-16.04944;3.69174;,
 -3.19185;-11.16739;2.61503;,
 -3.38209;-16.02532;2.51261;,
 3.36035;-6.99820;2.71059;,
 0.68806;-7.02286;3.91564;,
 -2.88860;-6.99820;2.71059;,
 2.72233;-3.69100;2.84862;,
 0.26799;-3.71621;4.08089;,
 -2.46030;-3.69100;2.84862;,
 1.70530;1.35983;2.02290;,
 -0.10722;-0.67787;5.11921;,
 -2.10829;1.35897;2.51583;,
 3.16558;-16.02532;2.51261;,
 0.28271;-17.29587;1.44358;,
 0.28271;-16.04944;3.69174;,
 -3.38209;-16.02532;2.51261;,
 1.37453;2.69965;0.95080;,
 1.70530;1.35983;2.02290;,
 -0.10722;-0.67787;5.11921;,
 -0.10722;3.53378;2.05399;,
 -2.10829;1.35897;2.51583;,
 -1.78183;2.74084;1.17740;;
 
 68;
 4;0,1,2,3;,
 4;1,4,5,2;,
 4;4,6,7,5;,
 4;3,2,8,9;,
 4;2,5,10,8;,
 4;5,7,11,10;,
 4;9,8,12,13;,
 4;8,10,14,12;,
 4;10,11,15,14;,
 4;13,12,16,17;,
 4;12,14,18,16;,
 4;14,15,19,18;,
 4;20,21,22,23;,
 3;24,20,23;,
 3;25,24,23;,
 3;26,27,28;,
 3;27,29,28;,
 3;28,29,30;,
 3;29,31,30;,
 3;31,29,32;,
 3;29,27,32;,
 3;32,27,33;,
 3;27,34,33;,
 4;34,35,32,33;,
 4;36,37,0,3;,
 4;38,36,3,9;,
 4;39,38,9,13;,
 4;40,39,13,17;,
 3;41,42,23;,
 3;43,41,23;,
 3;22,43,23;,
 4;44,45,25,23;,
 3;42,44,23;,
 4;30,46,26,28;,
 4;47,48,49,50;,
 4;50,49,51,52;,
 4;52,51,7,6;,
 4;48,53,54,49;,
 4;49,54,55,51;,
 4;51,55,11,7;,
 4;53,56,57,54;,
 4;54,57,58,55;,
 4;55,58,15,11;,
 4;56,59,60,57;,
 4;57,60,61,58;,
 4;58,61,19,15;,
 4;62,63,22,21;,
 3;64,63,62;,
 3;65,63,64;,
 3;30,31,66;,
 3;67,66,68;,
 3;66,31,68;,
 3;31,69,68;,
 3;68,69,70;,
 3;69,31,70;,
 3;31,32,71;,
 3;71,70,31;,
 4;70,71,32,35;,
 4;36,48,47,37;,
 4;38,53,48,36;,
 4;39,56,53,38;,
 4;40,59,56,39;,
 3;41,63,42;,
 3;43,63,41;,
 3;22,63,43;,
 4;44,63,65,45;,
 3;42,63,44;,
 4;30,66,67,46;;
 
 MeshMaterialList {
  1;
  68;
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
    "data\\TEXTURE\\yanki.jpg";
   }
  }
 }
 MeshNormals {
  69;
  0.601651;-0.374723;-0.705407;,
  0.040516;-0.529201;-0.847529;,
  -0.661915;0.002215;-0.749575;,
  0.713224;-0.032482;-0.700183;,
  0.052536;-0.021934;-0.998378;,
  -0.656678;0.013001;-0.754059;,
  0.718065;0.042013;-0.694707;,
  0.045151;-0.014356;-0.998877;,
  -0.668292;0.033952;-0.743124;,
  0.734970;0.171981;-0.655928;,
  0.049520;0.073757;-0.996046;,
  -0.707940;0.086307;-0.700979;,
  0.761015;0.415922;-0.497860;,
  0.215079;0.612039;-0.761019;,
  -0.748470;0.385584;-0.539553;,
  0.013940;-0.985677;-0.168065;,
  -0.006474;0.897128;-0.441724;,
  0.069055;0.863058;-0.500363;,
  0.536638;0.751189;-0.384364;,
  0.999572;-0.029243;-0.000000;,
  0.370454;-0.928851;0.000325;,
  -0.999389;0.034962;0.000000;,
  -0.999958;0.009162;0.000000;,
  0.996641;0.081894;-0.000000;,
  -0.994760;0.102234;-0.000000;,
  0.973672;0.227953;0.000000;,
  -0.989178;0.146721;-0.000000;,
  0.931540;0.363639;0.000000;,
  -0.964315;0.264758;0.000000;,
  0.001072;-0.999999;-0.000019;,
  -0.014880;-0.999889;-0.000016;,
  0.017955;-0.999839;-0.000016;,
  0.199859;-0.979825;0.000157;,
  -0.166605;-0.986024;0.000153;,
  0.632241;0.769051;-0.093977;,
  -0.068521;0.989520;0.127102;,
  -0.586622;0.779642;-0.219166;,
  0.601671;-0.375632;0.704906;,
  0.049808;-0.026746;0.998401;,
  -0.661915;0.002215;0.749575;,
  0.713224;-0.032482;0.700183;,
  0.052536;-0.021934;0.998378;,
  -0.656678;0.013001;0.754059;,
  0.718065;0.042013;0.694707;,
  0.045151;-0.014356;0.998877;,
  -0.668292;0.033952;0.743124;,
  0.761564;0.120932;0.636707;,
  0.027717;-0.042570;0.998709;,
  -0.751823;0.025770;0.658861;,
  0.787551;0.369785;0.492973;,
  0.190185;0.558437;0.807452;,
  -0.833288;0.368428;0.412179;,
  0.013890;-0.985775;0.167493;,
  1.000000;0.000000;0.000000;,
  -0.132521;0.898104;0.419341;,
  0.520766;0.769041;0.370647;,
  0.364796;-0.913646;-0.179374;,
  -0.236993;-0.914563;-0.327734;,
  -0.661079;0.547428;-0.513125;,
  -0.927412;0.368936;0.061586;,
  0.998806;-0.048857;-0.000000;,
  -0.309747;-0.950819;0.000317;,
  -0.598464;-0.128533;0.790772;,
  0.364052;-0.913817;0.180013;,
  0.021217;-0.874381;0.484776;,
  -0.236210;-0.915150;0.326657;,
  -0.628807;0.664367;0.404004;,
  0.198395;0.657311;0.727036;,
  -0.927412;0.368936;-0.061586;;
  68;
  4;0,1,4,3;,
  4;1,2,5,4;,
  4;2,22,21,5;,
  4;3,4,7,6;,
  4;4,5,8,7;,
  4;5,21,24,8;,
  4;6,7,10,9;,
  4;7,8,11,10;,
  4;8,24,26,11;,
  4;9,10,13,12;,
  4;10,11,14,13;,
  4;11,26,28,14;,
  4;56,20,32,15;,
  3;1,0,15;,
  3;57,1,15;,
  3;12,13,18;,
  3;13,16,18;,
  3;18,16,34;,
  3;16,35,34;,
  3;35,16,36;,
  3;16,58,36;,
  3;36,13,17;,
  3;13,14,17;,
  4;14,28,36,59;,
  4;19,60,0,3;,
  4;23,19,3,6;,
  4;25,23,6,9;,
  4;27,25,9,12;,
  3;29,30,15;,
  3;31,29,15;,
  3;32,31,15;,
  4;33,61,57,15;,
  3;30,33,15;,
  4;34,27,12,18;,
  4;37,40,41,38;,
  4;38,41,42,39;,
  4;39,42,21,22;,
  4;40,43,44,41;,
  4;41,44,45,42;,
  4;42,45,24,21;,
  4;43,46,47,44;,
  4;44,47,48,45;,
  4;45,48,26,24;,
  4;46,49,50,47;,
  4;47,62,51,48;,
  4;48,51,28,26;,
  4;63,52,32,20;,
  3;64,52,37;,
  3;65,52,64;,
  3;34,35,55;,
  3;49,55,50;,
  3;55,35,50;,
  3;53,53,53;,
  3;50,66,51;,
  3;66,35,51;,
  3;35,36,54;,
  3;54,67,35;,
  4;51,68,36,28;,
  4;19,40,37,60;,
  4;23,43,40,19;,
  4;25,46,43,23;,
  4;27,49,46,25;,
  3;29,52,30;,
  3;31,52,29;,
  3;32,52,31;,
  4;33,52,65,61;,
  3;30,52,33;,
  4;34,55,49,27;;
 }
 MeshTextureCoords {
  72;
  0.141458;0.880727;,
  0.145749;0.880727;,
  0.145749;0.867821;,
  0.141458;0.867821;,
  0.150041;0.880727;,
  0.150041;0.867821;,
  0.153828;0.880727;,
  0.153898;0.867821;,
  0.145749;0.854915;,
  0.141458;0.854915;,
  0.150041;0.854915;,
  0.154039;0.854915;,
  0.145749;0.842009;,
  0.141458;0.842009;,
  0.150041;0.842009;,
  0.154150;0.842009;,
  0.145749;0.829103;,
  0.141458;0.829103;,
  0.150041;0.829103;,
  0.154252;0.829103;,
  0.126286;0.887011;,
  0.120739;0.900403;,
  0.136141;0.902829;,
  0.141458;0.903041;,
  0.141458;0.880727;,
  0.156630;0.887011;,
  0.126286;0.822947;,
  0.141458;0.829232;,
  0.129055;0.820335;,
  0.141458;0.823326;,
  0.131371;0.818150;,
  0.141458;0.818455;,
  0.151344;0.817960;,
  0.154155;0.820612;,
  0.156630;0.822947;,
  0.162798;0.808057;,
  0.137601;0.867821;,
  0.137670;0.880727;,
  0.137460;0.854915;,
  0.137349;0.842009;,
  0.137267;0.829103;,
  0.141458;0.903548;,
  0.141965;0.903520;,
  0.140951;0.903520;,
  0.146775;0.902829;,
  0.162177;0.900403;,
  0.120148;0.808128;,
  0.141458;0.880727;,
  0.141458;0.867821;,
  0.145749;0.867821;,
  0.145749;0.880727;,
  0.150041;0.867821;,
  0.150041;0.880727;,
  0.141458;0.854915;,
  0.145749;0.854915;,
  0.150041;0.854915;,
  0.141458;0.842009;,
  0.145749;0.842009;,
  0.150041;0.842009;,
  0.141458;0.829103;,
  0.145749;0.829103;,
  0.150041;0.829103;,
  0.126286;0.887011;,
  0.141458;0.903041;,
  0.141458;0.880727;,
  0.156630;0.887011;,
  0.129055;0.820335;,
  0.126286;0.822947;,
  0.141458;0.829232;,
  0.141458;0.823326;,
  0.156630;0.822947;,
  0.154155;0.820612;;
 }
}