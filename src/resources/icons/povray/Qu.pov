light_source {
   <0.566667, 0.466667, -0.533333>, rgb <1, 1, 1>
   cylinder
   radius 70
   falloff 70
   point_at <0.133333, 0.0333333, 0.733333>
}

text {
   //*PMName labelQu
   ttf "/home/martin/arialbd.ttf"
   "Qu"
   1, <0, 0>
   
   pigment {
      color rgb <0.0196078, 0.67451, 1>
   }
   scale <1, 1, 0.3>
   rotate <0, 0, 0>
   translate <0, 0, 0>
}

global_settings {
   assumed_gamma 1.5
   noise_generator 2
}

light_source {
   <4, 5, -5>, rgb <1, 1, 1>
}

camera {
   perspective
   location <0.633337, 1.03334, -1.33333>
   sky <0, 1, 0>
   direction <0, 0, 1>
   right <1, 0, 0>
   up <0, 1, 0>
   look_at <0.633333, 0.3, 0.26667>
}

plane {
   <0, 1, 0>, 0
   
   pigment {
      color rgb <0.72549, 0.72549, 0.72549>
   }
   
   finish {
      diffuse 0.6
      brilliance 1
      phong 20
      metallic 1
      specular 30
      conserve_energy
      
      reflection {
         rgb <0, 0, 0>, rgb <1, 1, 1>
      }
   }
   scale 1
   rotate <0, 0, 0>
   translate y*(-0.1)
}