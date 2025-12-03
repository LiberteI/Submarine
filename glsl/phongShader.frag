
 
// incoming varying 
varying vec3 N;
varying vec3 v;    

void main (void)  
{  

   // vector to the light source
   vec3 L = normalize(gl_LightSource[0].position.xyz - v); 
   
   // normalized v vector  
   vec3 E = normalize(-v);  
   
   // reflection vector
   vec3 R = normalize(-reflect(L,N));  
 
   //calculate Ambient incidence Term  
   vec4 Iambient = gl_FrontLightProduct[0].ambient;    

   //calculate Diffuse incidence Term  
   vec4 Idiffuse = gl_FrontLightProduct[0].diffuse * max(dot(N,L), 0.0);
   // keep it between 0 and 1
   Idiffuse = clamp(Idiffuse, 0.0, 1.0);     
   
   // calculate Specular Term
   vec4 Ispecular = gl_FrontLightProduct[0].specular * pow(max(dot(R,E),0.0), gl_FrontMaterial.shininess);
   // keep it between 0 and 1
   Ispecular = clamp(Ispecular, 0.0, 1.0); 

   // write Total Color to the fragment color
   // note:  gl_FrontLightModelProduct.sceneColor = gl_FrontMaterial.emission + gl_FrontMaterial.ambient * gl_LightModel.ambient
   gl_FragColor = gl_FrontLightModelProduct.sceneColor + Iambient + Idiffuse + Ispecular; 

}
 