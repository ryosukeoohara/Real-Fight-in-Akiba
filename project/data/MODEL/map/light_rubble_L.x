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
 143;
 -5.98026;0.13932;-27.62768;,
 -7.30620;0.13932;-27.62768;,
 -7.30620;5.59941;-30.48072;,
 -5.98026;5.59941;-30.48072;,
 -5.46021;5.59944;-30.48070;,
 -5.46034;0.13937;-27.62768;,
 5.66632;0.14520;-27.65937;,
 5.18184;0.14520;-27.65937;,
 5.18184;5.52874;-30.09936;,
 5.66632;5.55730;-30.25334;,
 6.67362;5.52869;-30.09895;,
 6.67356;0.14520;-27.65937;,
 -7.53152;102.36128;29.21138;,
 -9.07926;102.36128;29.21138;,
 -7.30620;0.13932;-27.62768;,
 -5.98026;0.13932;-27.62768;,
 -9.07926;102.36128;29.21138;,
 -9.07927;113.89387;31.34880;,
 -7.30620;5.59941;-30.48072;,
 -7.30620;0.13932;-27.62768;,
 5.49346;102.49809;28.47303;,
 5.18184;0.14520;-27.65937;,
 5.66632;0.14520;-27.65937;,
 6.05889;102.49809;28.47303;,
 -9.07927;113.89387;31.34880;,
 -7.53152;113.89387;31.34880;,
 -5.98026;5.59941;-30.48072;,
 -7.30620;5.59941;-30.48072;,
 7.23456;102.49809;28.47303;,
 -6.92466;113.89387;31.34880;,
 -5.46021;5.59944;-30.48070;,
 7.23456;102.49809;28.47303;,
 6.67356;0.14520;-27.65937;,
 6.67362;5.52869;-30.09895;,
 7.23456;114.03069;30.61048;,
 7.23456;114.03069;30.61048;,
 5.66632;5.55730;-30.25334;,
 6.05889;114.03069;30.61048;,
 5.47313;114.03069;30.61048;,
 5.18184;5.52874;-30.09936;,
 -6.98072;113.89370;31.34873;,
 -7.12781;102.36146;29.21140;,
 -5.69581;0.13954;-27.62763;,
 -5.50838;5.59924;-30.48074;,
 5.45873;102.49809;28.47303;,
 5.49728;114.03069;30.61048;,
 5.15210;5.52873;-30.09934;,
 5.15210;0.14518;-27.65933;,
 -5.58766;5.36020;-30.46614;,
 -5.66661;0.06348;-27.61222;,
 -5.66661;23.01273;-14.41456;,
 -5.66661;36.45384;-13.44106;,
 -5.66661;0.06348;-27.61222;,
 5.53746;0.26547;-27.68825;,
 5.26039;24.44864;-14.56854;,
 -5.66661;23.01273;-14.41456;,
 5.53746;0.26547;-27.68825;,
 5.69295;5.62239;-30.23161;,
 5.26046;36.57943;-12.29936;,
 5.26039;24.44864;-14.56854;,
 5.69295;5.62239;-30.23161;,
 -5.58766;5.36020;-30.46614;,
 -5.66661;36.45384;-13.44106;,
 5.26046;36.57943;-12.29936;,
 -5.66661;0.06348;-27.61222;,
 -5.58766;5.36020;-30.46614;,
 -8.08638;257.77083;49.49929;,
 -11.15331;258.26084;48.59749;,
 -8.57527;258.03023;48.85380;,
 -5.80090;257.91543;48.85380;,
 7.29435;230.50240;16.77819;,
 4.51816;230.09551;17.58627;,
 5.60069;258.32769;49.32692;,
 11.04093;258.51641;48.85380;,
 -6.73445;7.09511;-25.01319;,
 -4.91035;4.46154;-27.28305;,
 -4.90765;-1.65278;-24.94104;,
 -6.73182;0.98077;-22.66971;,
 -0.50662;3.35191;-28.21698;,
 -0.50375;-2.76233;-25.87504;,
 3.89725;4.41606;-27.26773;,
 3.90010;-1.69808;-24.92559;,
 5.72145;7.03057;-24.99171;,
 5.72407;0.91660;-22.64917;,
 3.89725;9.66417;-22.72099;,
 3.90010;3.54960;-20.37788;,
 -0.50662;10.77411;-21.78717;,
 -0.50375;4.65950;-19.44458;,
 -4.91035;9.70956;-22.73649;,
 -6.73445;7.09511;-25.01319;,
 -6.73182;0.98077;-22.66971;,
 -4.90765;3.59547;-20.39399;,
 -0.50659;7.06265;-25.00171;,
 -0.50659;7.06265;-25.00171;,
 -0.50659;7.06265;-25.00171;,
 -0.50659;7.06265;-25.00171;,
 -0.50659;7.06265;-25.00171;,
 -0.50659;7.06265;-25.00171;,
 -0.50659;7.06265;-25.00171;,
 -3.61778;0.92579;-22.57902;,
 -2.70563;-0.39103;-23.71364;,
 -0.50375;0.90975;-22.57342;,
 -2.70840;10.24183;-22.26145;,
 -0.50659;7.06265;-25.00171;,
 -2.70563;2.23308;-21.44030;,
 -3.61778;0.92579;-22.57902;,
 -0.50375;0.90975;-22.57342;,
 -0.50375;2.76537;-20.96609;,
 -2.70563;2.23308;-21.44030;,
 -0.50375;0.90975;-22.57342;,
 1.69816;2.21039;-21.43285;,
 -0.50375;2.76537;-20.96609;,
 -0.50375;0.90975;-22.57342;,
 2.61010;0.89358;-22.56742;,
 1.69816;2.21039;-21.43285;,
 -0.50375;0.90975;-22.57342;,
 1.69816;-0.41368;-23.70644;,
 2.61010;0.89358;-22.56742;,
 -0.50375;0.90975;-22.57342;,
 -0.50375;-0.94582;-24.18129;,
 -0.50375;0.90975;-22.57342;,
 -2.70563;-0.39103;-23.71364;,
 -0.50375;-0.94582;-24.18129;,
 1.69816;-0.41368;-23.70644;,
 -0.50375;0.90975;-22.57342;,
 -3.80665;2.93375;-20.96082;,
 -0.50375;3.73184;-20.24755;,
 -3.80665;2.93375;-20.96082;,
 -5.17482;0.97285;-22.66827;,
 -3.80665;-1.00231;-24.37043;,
 -5.17482;0.97285;-22.66827;,
 -3.80665;-1.00231;-24.37043;,
 -0.50375;-1.83454;-25.07132;,
 -0.50375;-1.83454;-25.07132;,
 2.79910;-1.03668;-24.35893;,
 4.16714;0.92448;-22.65163;,
 2.79910;-1.03668;-24.35893;,
 4.16714;0.92448;-22.65163;,
 2.79910;2.89944;-20.94806;,
 2.79910;2.89944;-20.94806;,
 -0.50375;3.73184;-20.24755;,
 -3.80939;9.97573;-22.49876;,
 -4.35847;6.85945;-21.61060;;
 
 66;
 4;0,1,2,3;,
 4;0,3,4,5;,
 4;6,7,8,9;,
 4;6,9,10,11;,
 4;12,13,14,15;,
 4;16,17,18,19;,
 4;20,21,22,23;,
 4;24,25,26,27;,
 4;23,22,11,28;,
 4;25,29,30,26;,
 4;31,32,33,34;,
 4;35,10,36,37;,
 4;38,37,36,39;,
 4;40,41,42,43;,
 4;44,45,46,47;,
 4;48,49,50,51;,
 4;52,53,54,55;,
 4;56,57,58,59;,
 4;60,61,62,63;,
 4;60,53,64,65;,
 4;41,12,15,52;,
 4;66,67,68,69;,
 4;70,71,72,73;,
 4;74,75,76,77;,
 4;75,78,79,76;,
 4;78,80,81,79;,
 4;80,82,83,81;,
 4;82,84,85,83;,
 4;84,86,87,85;,
 4;88,89,90,91;,
 3;92,75,74;,
 3;93,78,75;,
 3;94,80,78;,
 3;95,82,80;,
 3;96,84,82;,
 3;97,86,84;,
 3;98,89,88;,
 3;99,100,101;,
 3;102,86,103;,
 3;104,105,106;,
 3;107,108,109;,
 3;110,111,112;,
 3;113,114,115;,
 3;116,117,118;,
 3;119,120,121;,
 3;122,123,124;,
 4;91,125,126,87;,
 4;127,91,90,128;,
 4;77,76,129,130;,
 4;131,76,79,132;,
 4;133,79,81,134;,
 4;83,135,136,81;,
 4;137,83,85,138;,
 4;139,85,87,140;,
 4;114,113,137,138;,
 4;117,116,136,135;,
 4;123,122,133,134;,
 4;119,121,131,132;,
 4;99,130,129,100;,
 4;105,104,127,128;,
 4;108,107,126,125;,
 4;111,110,139,140;,
 4;91,102,141,142;,
 4;91,142,141,88;,
 4;91,87,86,102;,
 4;88,141,102,103;;
 
 MeshMaterialList {
  3;
  66;
  1,
  1,
  1,
  1,
  1,
  1,
  1,
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
  0,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
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
  0,
  0,
  0,
  0,
  0,
  0,
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
  0,
  0,
  0,
  0,
  0,
  0,
  0,
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
  84;
  0.000000;0.018634;-0.999826;,
  0.000000;-0.463114;-0.886299;,
  0.062599;-0.421642;-0.904599;,
  -0.185712;0.746089;-0.639423;,
  0.076240;0.036858;-0.996408;,
  0.000046;-0.463114;-0.886299;,
  -0.044527;-0.938172;-0.343294;,
  0.000024;0.018635;-0.999826;,
  -0.127370;-0.418935;-0.899038;,
  -0.154283;0.036372;-0.987357;,
  0.000023;-0.463114;-0.886299;,
  0.000012;0.018635;-0.999826;,
  -0.032533;-0.422200;-0.905919;,
  -0.039381;0.036861;-0.998544;,
  -0.039821;-0.485919;0.873096;,
  0.000000;-0.485964;0.873979;,
  0.000000;-0.480855;0.876800;,
  -0.999572;-0.000271;-0.029252;,
  -0.000002;0.495818;-0.868426;,
  -0.000004;0.495818;-0.868426;,
  0.999954;0.000037;-0.009638;,
  -0.147897;0.483830;-0.862575;,
  -0.037580;0.488560;-0.871721;,
  0.998240;-0.018127;0.056467;,
  -0.999996;0.002389;0.001280;,
  -0.999948;0.004041;-0.009362;,
  0.044837;-0.486924;0.872293;,
  0.999254;-0.008396;0.037687;,
  0.000000;0.495818;-0.868426;,
  0.073198;0.487293;-0.870165;,
  -0.017405;-0.486957;0.873252;,
  0.034810;0.021913;-0.999154;,
  0.045716;0.490707;-0.870125;,
  0.015669;-0.452063;-0.891848;,
  -0.079579;-0.485104;0.870828;,
  0.004691;0.654777;-0.755808;,
  -0.859229;-0.183370;-0.477599;,
  -0.610884;-0.283455;-0.739239;,
  -0.000214;-0.357697;-0.933837;,
  0.610498;-0.283081;-0.739700;,
  0.858950;-0.182855;-0.478297;,
  0.610892;0.283514;0.739210;,
  0.000221;0.357757;0.933815;,
  -0.564400;0.285071;0.774717;,
  -0.441093;0.319191;0.838781;,
  -0.004711;-0.654794;0.755793;,
  -0.025054;-0.673732;0.738551;,
  -0.033924;-0.654287;0.755485;,
  0.004662;0.654740;-0.755840;,
  -0.266821;0.341431;0.901239;,
  -0.025473;-0.634356;0.772621;,
  -0.004626;-0.625992;0.779816;,
  0.016038;-0.634506;0.772752;,
  0.024270;-0.654555;0.755624;,
  0.015457;-0.673938;0.738626;,
  -0.004753;-0.681695;0.731622;,
  -0.033727;-0.654314;0.755470;,
  -0.025439;-0.634356;0.772622;,
  -0.004697;-0.625960;0.779841;,
  0.016143;-0.634548;0.772715;,
  0.024447;-0.654590;0.755588;,
  0.015550;-0.673908;0.738651;,
  -0.004798;-0.681696;0.731620;,
  -0.025029;-0.673751;0.738534;,
  0.859261;0.183366;0.477544;,
  -0.858956;0.182901;0.478270;,
  0.004765;0.654803;-0.755785;,
  0.004805;0.654781;-0.755803;,
  0.004696;0.654841;-0.755752;,
  0.004627;0.654803;-0.755786;,
  0.004587;0.654790;-0.755797;,
  0.004655;0.654750;-0.755831;,
  0.004638;0.654685;-0.755888;,
  0.004788;0.654797;-0.755790;,
  -0.004554;-0.654824;0.755768;,
  -0.004795;-0.654657;0.755911;,
  -0.004340;-0.654825;0.755768;,
  -0.004561;-0.654780;0.755806;,
  -0.004695;-0.654861;0.755735;,
  -0.004602;-0.654864;0.755733;,
  -0.004500;-0.654899;0.755703;,
  -0.004613;-0.654774;0.755811;,
  -0.321246;0.344525;0.882102;,
  0.004771;0.654796;-0.755791;;
  66;
  4;10,1,0,11;,
  4;10,11,7,5;,
  4;12,8,9,13;,
  4;12,13,4,2;,
  4;14,15,15,14;,
  4;17,17,17,17;,
  4;16,16,16,16;,
  4;28,18,11,0;,
  4;16,16,16,16;,
  4;18,19,7,11;,
  4;20,20,20,20;,
  4;29,4,13,22;,
  4;21,22,13,9;,
  4;23,23,23,23;,
  4;24,24,24,24;,
  4;25,25,25,25;,
  4;30,26,26,26;,
  4;27,27,27,27;,
  4;31,31,32,32;,
  4;31,33,33,31;,
  4;34,14,14,30;,
  4;6,6,6,6;,
  4;3,3,3,3;,
  4;36,37,37,36;,
  4;37,38,38,37;,
  4;38,39,39,38;,
  4;39,40,40,39;,
  4;64,41,41,64;,
  4;41,42,42,41;,
  4;43,65,65,44;,
  3;35,66,67;,
  3;35,68,66;,
  3;35,69,68;,
  3;35,70,69;,
  3;35,71,70;,
  3;35,72,71;,
  3;35,67,73;,
  3;47,46,45;,
  3;48,72,35;,
  3;50,47,45;,
  3;51,50,45;,
  3;52,51,45;,
  3;53,52,45;,
  3;54,53,45;,
  3;55,45,46;,
  3;55,54,45;,
  4;74,57,58,75;,
  4;57,74,76,56;,
  4;76,77,63,56;,
  4;63,77,78,62;,
  4;62,78,79,61;,
  4;80,60,61,79;,
  4;60,80,81,59;,
  4;59,81,75,58;,
  4;52,53,60,59;,
  4;53,54,61,60;,
  4;54,55,62,61;,
  4;55,46,63,62;,
  4;47,56,63,46;,
  4;47,50,57,56;,
  4;50,51,58,57;,
  4;51,52,59,58;,
  4;44,82,49,49;,
  4;44,49,49,43;,
  4;44,42,42,82;,
  4;73,83,48,35;;
 }
 MeshTextureCoords {
  143;
  1.000000;0.899200;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.899200;,
  0.000000;0.859680;,
  1.000000;0.859680;,
  1.000000;0.076570;,
  1.000000;0.113400;,
  0.000000;0.113400;,
  0.000000;0.076570;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.100800;0.044200;,
  0.000000;0.044200;,
  0.000000;0.000000;,
  0.100800;0.000000;,
  1.000000;0.044200;,
  0.000000;0.044200;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.886600;0.044200;,
  0.886600;0.000000;,
  0.923430;0.000000;,
  0.923430;0.044200;,
  1.000000;0.044200;,
  0.899200;0.044200;,
  0.899200;0.000000;,
  1.000000;0.000000;,
  1.000000;0.044200;,
  0.859680;0.044200;,
  0.859680;0.000000;,
  0.000000;0.044200;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;0.044200;,
  0.000000;0.044200;,
  0.076570;0.000000;,
  0.076570;0.044200;,
  0.113400;0.044200;,
  0.113400;0.000000;,
  1.000000;0.044200;,
  0.000000;0.044200;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;0.044200;,
  0.000000;0.044200;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.140320;0.814250;,
  1.000000;0.907120;,
  1.000000;0.907120;,
  0.140320;0.814250;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.886600;0.814250;,
  0.000000;0.814250;,
  0.000000;0.000000;,
  0.125000;0.000000;,
  0.125000;1.000000;,
  0.000000;1.000000;,
  0.250000;0.000000;,
  0.250000;1.000000;,
  0.375000;0.000000;,
  0.375000;1.000000;,
  0.500000;0.000000;,
  0.500000;1.000000;,
  0.625000;0.000000;,
  0.625000;1.000000;,
  0.750000;0.000000;,
  0.750000;1.000000;,
  0.875000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.875000;1.000000;,
  0.062500;0.000000;,
  0.187500;0.000000;,
  0.312500;0.000000;,
  0.437500;0.000000;,
  0.562500;0.000000;,
  0.687500;0.000000;,
  0.937500;0.000000;,
  0.031250;1.000000;,
  0.093750;1.000000;,
  0.062500;1.000000;,
  0.812500;0.000000;,
  0.812500;0.000000;,
  0.906250;1.000000;,
  0.968750;1.000000;,
  0.937500;1.000000;,
  0.781250;1.000000;,
  0.843750;1.000000;,
  0.812500;1.000000;,
  0.656250;1.000000;,
  0.718750;1.000000;,
  0.687500;1.000000;,
  0.531250;1.000000;,
  0.593750;1.000000;,
  0.562500;1.000000;,
  0.406250;1.000000;,
  0.468750;1.000000;,
  0.437500;1.000000;,
  0.218750;1.000000;,
  0.187500;1.000000;,
  0.156250;1.000000;,
  0.281250;1.000000;,
  0.343750;1.000000;,
  0.312500;1.000000;,
  0.859380;1.000000;,
  0.765630;1.000000;,
  0.890620;1.000000;,
  0.984370;1.000000;,
  0.109380;1.000000;,
  0.015630;1.000000;,
  0.140620;1.000000;,
  0.234380;1.000000;,
  0.265620;1.000000;,
  0.359380;1.000000;,
  0.484380;1.000000;,
  0.390620;1.000000;,
  0.515630;1.000000;,
  0.609380;1.000000;,
  0.640620;1.000000;,
  0.734370;1.000000;,
  0.843750;0.000000;,
  0.859380;0.500000;;
 }
}
