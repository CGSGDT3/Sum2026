/**/
#version 460

layout(location = 0) out vec4 OutColor;
layout(binding = 1) uniform sampler2D Tex;

in vec4 DrawColor;
in vec3 DrawNormal;
in vec3 DrawPos;
in vec3 DrawPosOrg;
in vec2 DrawTexCoord;

uniform vec3 CamLoc;
uniform float Time, GlobalTime;
uniform vec3 Ka, Kd, Ks;
uniform float Ph; 

void main( void )
{
 // if (DrawPos.y + DrawPos.x > 30 * abs(sin(Time)))
   // ;//discard;

 // if (DrawPosOrg.x * DrawPosOrg.x + DrawPosOrg.z *  DrawPosOrg.z < 1 + 2 * abs(sin(Time)))
  //  ;//discard;
  /*       

  OutColor = DrawColor;

  OutColor = vec4(vec3(1, 0.8, 0.1) * dot(N, L) + vec3(1, 0.8, 1)  * dot(N, L1), 1);
  vec3 L = normalize(vec3(10 * sin(0.8 * Time), 1, 1));
  vec3 L1 = normalize(vec3(1, 1, 10 * sin(0.5 * Time + 1)));

  vec3 N = normalize(DrawNormal);
  vec3 color =
    vec3(1, 1, 0) * max(0.1, dot(N, L)) +
    vec3(0, 1, 1) * max(0.1, dot(N, L1));
  */
  /*
  color += vec3(0, 1, 0) * pow(abs(sin(3 * GlobalTime + 18 * DrawPos.x)), 300);
  color += vec3(0, 0, 1) * pow(abs(sin(3 * GlobalTime + -18 * DrawPos.z)), 300);
  color += vec3(1, 0, 0) * pow(abs(sin(3 * GlobalTime + -18 * DrawPos.y)), 300);
  */

  vec3 color = texture(Tex, DrawTexCoord).rgb;  
  vec3 N = normalize(DrawNormal);
  vec3 L = normalize(1 + vec3(sin(1 * Time), 1, 1));

  vec3 LPos = vec3(cos(Time), 1, sin(Time));
  L = normalize(LPos - DrawPos);
  vec3 D = normalize(vec3(1) - LPos);//vec3(0, -1, 0);
  float F = 1, a = radians(36.30), b = radians(27.0);

  float ld = dot(-L, D), start = cos(a), end = cos(b);
  if (ld < start)
    F = 0;
  else if (ld < end)
    F = 1 - (ld - end) / (start - end);
  else
    F = 1; 

  // Ka
  color += Ka;// * DrawColor.rgb;

  // Kd
  color += F * Kd * max(0, dot(N, L));// * DrawColor.rgb;

  // Ks
  vec3 V = normalize(DrawPos - CamLoc);
  vec3 R = reflect(V, N);
  color += F * Ks * max(0, pow(dot(R, L), Ph));

  OutColor = vec4(color, 1);
}