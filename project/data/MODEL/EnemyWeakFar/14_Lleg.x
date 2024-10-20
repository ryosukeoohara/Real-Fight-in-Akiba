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
 84;
 -2.32609;-5.42578;-4.14117;,
 -2.31214;-4.29053;-4.14117;,
 -1.64712;-4.29871;-5.74677;,
 -1.66108;-5.43395;-5.74677;,
 -0.04164;-4.31844;-6.41184;,
 -0.05559;-5.45369;-6.41184;,
 1.56384;-4.33818;-5.74677;,
 1.54989;-5.47342;-5.74677;,
 2.22886;-4.34635;-4.14117;,
 2.21490;-5.48160;-4.14117;,
 2.22615;-4.34047;-0.03084;,
 2.21220;-5.47572;-0.03084;,
 -2.22749;-3.15615;-0.03084;,
 -1.58318;-3.16407;-1.58646;,
 -0.02768;-3.18319;-2.23081;,
 1.52781;-3.20231;-1.58646;,
 2.17212;-3.21023;-0.03084;,
 -2.21097;-3.03558;-0.03084;,
 -1.56735;-3.04349;-1.58480;,
 -0.01351;-3.06259;-2.22847;,
 1.54033;-3.08169;-1.58480;,
 2.18396;-3.08960;-0.03084;,
 -2.19746;-2.63145;-0.03084;,
 -1.55384;-2.63936;-1.58480;,
 0.00000;-2.65846;-2.22847;,
 1.55384;-2.67756;-1.58480;,
 2.19746;-2.68548;-0.03084;,
 -2.32609;-5.42578;-4.14117;,
 -0.05559;-5.45369;-0.03084;,
 -2.42147;-5.43166;-0.03084;,
 -0.05559;-5.45369;-6.41184;,
 -1.66108;-5.43395;-5.74677;,
 1.54989;-5.47342;-5.74677;,
 -2.19746;-2.63145;-0.03084;,
 -1.01890;-0.96161;-0.03084;,
 -0.81404;-0.96413;-0.84494;,
 -1.55384;-2.63936;-1.58480;,
 0.00000;-0.97414;-1.16773;,
 0.00000;-2.65846;-2.22847;,
 0.76507;-0.98354;-0.79597;,
 1.55384;-2.67756;-1.58480;,
 1.02580;-0.98675;-0.03084;,
 2.19746;-2.68548;-0.03084;,
 -2.40752;-4.29641;-0.03084;,
 -2.42147;-5.43166;-0.03084;,
 2.21490;-5.48160;-4.14117;,
 2.21220;-5.47572;-0.03084;,
 -2.32609;-5.42578;0.72545;,
 -1.66108;-5.43395;2.33105;,
 -1.64712;-4.29871;2.33105;,
 -2.31214;-4.29053;0.72545;,
 -0.05559;-5.45369;2.99612;,
 -0.04164;-4.31844;2.99612;,
 1.54989;-5.47342;2.33105;,
 1.56384;-4.33818;2.33105;,
 2.21490;-5.48160;0.72545;,
 2.22886;-4.34635;0.72545;,
 -1.58318;-3.16407;1.52477;,
 -0.02768;-3.18319;2.16913;,
 1.52781;-3.20231;1.52477;,
 -1.56735;-3.04349;1.52312;,
 -0.01351;-3.06259;2.16679;,
 1.54033;-3.08169;1.52312;,
 -1.55384;-2.63936;1.52312;,
 0.00000;-2.65846;2.16679;,
 1.55384;-2.67756;1.52312;,
 -2.32609;-5.42578;0.72545;,
 -0.05559;-5.45369;2.99612;,
 -1.66108;-5.43395;2.33105;,
 1.54989;-5.47342;2.33105;,
 -1.55384;-2.63936;1.52312;,
 -0.69239;-0.96563;0.66160;,
 0.00000;-2.65846;2.16679;,
 0.00000;-0.97414;1.00072;,
 1.55384;-2.67756;1.52312;,
 0.68163;-0.98252;0.65084;,
 2.21490;-5.48160;0.72545;,
 0.00000;0.08667;-0.03084;,
 -1.66108;-5.43395;-5.74677;,
 2.21490;-5.48160;-4.14117;,
 -1.66108;-5.43395;2.33105;,
 -2.32609;-5.42578;0.72545;,
 2.21490;-5.48160;0.72545;,
 1.54989;-5.47342;2.33105;;
 
 72;
 4;0,1,2,3;,
 4;3,2,4,5;,
 4;5,4,6,7;,
 4;7,6,8,9;,
 4;9,8,10,11;,
 3;12,13,1;,
 3;13,2,1;,
 4;2,13,14,4;,
 4;4,14,15,6;,
 3;6,15,8;,
 3;8,15,16;,
 3;8,16,10;,
 4;12,17,18,13;,
 4;13,18,19,14;,
 4;14,19,20,15;,
 4;15,20,21,16;,
 4;17,22,23,18;,
 4;18,23,24,19;,
 4;19,24,25,20;,
 4;20,25,26,21;,
 3;27,28,29;,
 3;30,28,31;,
 3;32,28,30;,
 4;33,34,35,36;,
 4;36,35,37,38;,
 4;38,37,39,40;,
 4;40,39,41,42;,
 4;43,1,0,44;,
 3;12,1,43;,
 3;28,45,46;,
 4;47,48,49,50;,
 4;48,51,52,49;,
 4;51,53,54,52;,
 4;53,55,56,54;,
 4;55,11,10,56;,
 3;49,57,50;,
 3;57,12,50;,
 4;49,52,58,57;,
 4;52,54,59,58;,
 3;16,59,56;,
 3;59,54,56;,
 3;56,10,16;,
 4;12,57,60,17;,
 4;57,58,61,60;,
 4;58,59,62,61;,
 4;59,16,21,62;,
 4;17,60,63,22;,
 4;60,61,64,63;,
 4;61,62,65,64;,
 4;62,21,26,65;,
 3;66,29,28;,
 3;67,68,28;,
 3;69,67,28;,
 4;33,70,71,34;,
 4;70,72,73,71;,
 4;72,74,75,73;,
 4;74,42,41,75;,
 4;43,44,47,50;,
 3;12,43,50;,
 3;28,46,76;,
 3;71,73,77;,
 3;34,71,77;,
 3;35,34,77;,
 3;37,35,77;,
 3;39,37,77;,
 3;41,39,77;,
 3;75,41,77;,
 3;73,75,77;,
 3;78,28,27;,
 3;79,28,32;,
 3;80,81,28;,
 3;82,83,28;;
 
 MeshMaterialList {
  1;
  72;
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
  73;
  -0.978384;0.012025;-0.206446;,
  -0.707054;0.008691;-0.707106;,
  0.000001;0.000001;-1.000000;,
  0.707054;-0.008691;-0.707106;,
  0.980777;-0.012056;-0.194760;,
  -0.889138;0.414045;-0.194937;,
  -0.602950;0.567248;-0.560964;,
  0.730764;0.316984;-0.604570;,
  0.907803;0.376030;-0.185727;,
  -0.949479;0.310564;0.045152;,
  -0.575774;0.665879;-0.474436;,
  0.011862;0.964980;-0.262056;,
  0.406369;0.882335;-0.237380;,
  0.981430;0.189912;0.026998;,
  -0.996406;0.084709;0.000000;,
  -0.704983;0.062800;-0.706438;,
  0.000268;0.009676;-0.999953;,
  0.705972;-0.043551;-0.706900;,
  0.997848;-0.065572;0.000000;,
  -0.947200;0.320054;0.019402;,
  -0.680683;0.302154;-0.667363;,
  0.010152;0.283872;-0.958808;,
  0.688474;0.282063;-0.668165;,
  0.957284;0.288834;0.013491;,
  -0.011365;-0.999935;0.000082;,
  -0.619058;0.726585;0.298062;,
  -0.009311;-0.999950;0.003651;,
  0.999923;-0.012292;-0.001498;,
  -0.995157;0.084237;0.050663;,
  0.999842;0.017737;-0.001404;,
  -0.966567;0.011879;0.256139;,
  -0.707054;0.008691;0.707106;,
  0.000001;0.000001;1.000000;,
  0.707054;-0.008691;0.707106;,
  0.981067;-0.012060;0.193290;,
  -0.947095;0.162129;0.276994;,
  -0.693142;0.246480;0.677348;,
  0.003855;0.313603;0.949546;,
  0.698992;0.229367;0.677348;,
  0.965904;0.123241;0.227685;,
  -0.733667;0.298417;0.610475;,
  0.004122;0.322744;0.946477;,
  0.751865;0.223916;0.620129;,
  -0.704984;0.062785;0.706438;,
  0.000268;0.009656;0.999953;,
  0.705972;-0.043565;0.706899;,
  -0.668949;0.321068;0.670390;,
  0.005053;0.312917;0.949767;,
  0.677160;0.297889;0.672842;,
  0.012326;0.645353;0.763785;,
  -0.546953;0.632865;0.548018;,
  -0.769473;0.636229;0.055898;,
  -0.558377;0.651483;-0.513600;,
  0.028035;0.641099;-0.766946;,
  0.573157;0.636281;-0.516370;,
  0.772296;0.634024;0.039667;,
  0.556717;0.625351;0.546811;,
  0.119699;0.958018;-0.260526;,
  0.000536;0.019359;-0.999813;,
  0.704895;-0.063466;-0.706466;,
  -0.010800;-0.999941;-0.000824;,
  -0.012291;-0.999925;0.000000;,
  -0.012292;-0.999925;-0.000001;,
  -0.012292;-0.999925;-0.000001;,
  -0.998620;0.012271;0.051062;,
  -0.011004;-0.999939;0.000711;,
  -0.009714;-0.999948;-0.003158;,
  -0.010801;-0.999932;0.004476;,
  -0.012293;-0.999925;0.000001;,
  -0.012290;-0.999925;0.000002;,
  -0.011003;-0.999932;-0.003868;,
  0.286368;0.768396;-0.572330;,
  0.530861;0.669608;0.519434;;
  72;
  4;0,5,6,1;,
  4;1,6,2,2;,
  4;2,2,7,3;,
  4;3,7,8,4;,
  4;4,8,29,27;,
  3;9,10,5;,
  3;10,6,5;,
  4;6,10,11,11;,
  4;11,11,12,57;,
  3;7,12,8;,
  3;8,12,13;,
  3;8,13,29;,
  4;9,14,15,10;,
  4;10,15,16,58;,
  4;58,16,17,59;,
  4;59,17,18,13;,
  4;14,19,20,15;,
  4;15,20,21,16;,
  4;16,21,22,17;,
  4;17,22,23,18;,
  3;60,24,26;,
  3;61,24,62;,
  3;63,24,61;,
  4;19,51,52,20;,
  4;20,52,53,21;,
  4;21,53,54,22;,
  4;22,54,55,23;,
  4;28,5,0,64;,
  3;9,5,28;,
  3;24,65,66;,
  4;30,31,36,35;,
  4;31,32,37,36;,
  4;32,33,38,37;,
  4;33,34,39,38;,
  4;34,27,29,39;,
  3;36,40,35;,
  3;40,9,35;,
  4;36,37,41,40;,
  4;37,38,42,41;,
  3;13,42,39;,
  3;42,38,39;,
  3;39,29,13;,
  4;9,40,43,14;,
  4;40,41,44,43;,
  4;41,42,45,44;,
  4;42,13,18,45;,
  4;14,43,46,19;,
  4;43,44,47,46;,
  4;44,45,48,47;,
  4;45,18,23,48;,
  3;67,26,24;,
  3;61,68,24;,
  3;69,61,24;,
  4;19,46,50,51;,
  4;46,47,49,50;,
  4;47,48,56,49;,
  4;48,23,55,56;,
  4;28,64,30,35;,
  3;9,28,35;,
  3;24,66,70;,
  3;50,49,25;,
  3;51,50,25;,
  3;52,51,25;,
  3;53,52,71;,
  3;54,53,71;,
  3;55,54,71;,
  3;56,55,72;,
  3;49,56,72;,
  3;62,24,60;,
  3;65,24,63;,
  3;68,67,24;,
  3;70,69,24;;
 }
 MeshTextureCoords {
  84;
  0.547016;0.512929;,
  0.547016;0.500739;,
  0.551069;0.500739;,
  0.551069;0.512929;,
  0.555122;0.500739;,
  0.555122;0.512929;,
  0.559175;0.500739;,
  0.559175;0.512929;,
  0.563228;0.500739;,
  0.563228;0.512929;,
  0.566143;0.500739;,
  0.566143;0.512929;,
  0.547016;0.488550;,
  0.551069;0.488550;,
  0.555122;0.488550;,
  0.559175;0.488550;,
  0.563228;0.488550;,
  0.547016;0.476360;,
  0.551069;0.476360;,
  0.555122;0.476360;,
  0.559175;0.476360;,
  0.563228;0.476360;,
  0.547016;0.464170;,
  0.551069;0.464170;,
  0.555122;0.464170;,
  0.559175;0.464170;,
  0.563228;0.464170;,
  0.530804;0.533195;,
  0.551069;0.534005;,
  0.535072;0.543499;,
  0.551069;0.512929;,
  0.536739;0.518865;,
  0.565399;0.518865;,
  0.530804;0.444027;,
  0.541673;0.444462;,
  0.543562;0.451920;,
  0.536739;0.458357;,
  0.551069;0.454902;,
  0.551069;0.464292;,
  0.558125;0.451494;,
  0.565399;0.458357;,
  0.560529;0.444459;,
  0.571334;0.444027;,
  0.544101;0.500739;,
  0.544101;0.512929;,
  0.571334;0.533195;,
  0.567066;0.543499;,
  0.547016;0.512929;,
  0.551069;0.512929;,
  0.551069;0.500739;,
  0.547016;0.500739;,
  0.555122;0.512929;,
  0.555122;0.500739;,
  0.559175;0.512929;,
  0.559175;0.500739;,
  0.563228;0.512929;,
  0.563228;0.500739;,
  0.551069;0.488550;,
  0.555122;0.488550;,
  0.559175;0.488550;,
  0.551069;0.476360;,
  0.555122;0.476360;,
  0.559175;0.476360;,
  0.551069;0.464170;,
  0.555122;0.464170;,
  0.559175;0.464170;,
  0.530804;0.533195;,
  0.551069;0.512929;,
  0.536739;0.518865;,
  0.565399;0.518865;,
  0.536739;0.458357;,
  0.544684;0.450862;,
  0.551069;0.464292;,
  0.551069;0.453969;,
  0.565399;0.458357;,
  0.557355;0.450768;,
  0.571334;0.533195;,
  0.551069;0.444837;,
  0.543904;0.526435;,
  0.561202;0.533600;,
  0.543904;0.526435;,
  0.540936;0.533600;,
  0.561202;0.533600;,
  0.558234;0.526435;;
 }
}