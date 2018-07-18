// ------------------------------------------------
// BASIC SETUP
// ------------------------------------------------

// Create an empty scene
var scene = new THREE.Scene();

// Create a basic perspective camera
var camera = new THREE.PerspectiveCamera( 75,5/3, 0.1, 1000 );
camera.position.x = 3;
camera.position.y = 0.5;
camera.position.z = 1;
camera.lookAt(0,0,0);
camera.rotateZ(Math.PI/2);
// Create a renderer with Antialiasing
var renderer = new THREE.WebGLRenderer({antialias:true});

// Configure renderer clear color
renderer.setClearColor("#ffffff");
let submodel = document.getElementById("submodel");

// Configure renderer size
renderer.setSize( 500, 300 );

var radius = 2.3;
var radials = 16;
var circles = 12;
var divisions = 64;

var gridHelper = new THREE.PolarGridHelper( radius, radials, circles, divisions, "#777777", "#bbbbbb" );
gridHelper.rotateZ(Math.PI/2);
gridHelper.rotateX(Math.PI/2);
scene.add( gridHelper );

var worldAxis = new THREE.AxesHelper(20);
scene.add(worldAxis);


var arrowHelper = false;

// Append Renderer to DOM
document.getElementById("submodel").appendChild( renderer.domElement );

let submesh;
let baseQuat = (new THREE.Quaternion(0,0,0,0)).setFromEuler(new THREE.Euler( 0, 0, Math.PI/2, 'XYZ' ));

var loader = new THREE.STLLoader();
loader.load( '/rsapp/mini.stl', function ( geometry ) {
  var material = new THREE.MeshPhongMaterial( { color: 0x008080, specular: 0x111111, shininess: 50 } );
  var mesh = new THREE.Mesh( geometry, material );

  mesh.position.set( 0, 0, 0 );
  mesh.setRotationFromQuaternion(baseQuat);
  mesh.scale.set( 0.5, 0.5, 0.5 );

  mesh.castShadow = true;
  mesh.receiveShadow = true;
  scene.add( mesh );
  submesh = mesh;
  
  var origin = new THREE.Vector3( 0, 0, 0 );
  var length = 2;
  var hex = 0xad42f4;
  
  arrowHelper = new THREE.ArrowHelper( new THREE.Vector3(0, 0, 1), origin, length, hex );
  scene.add( arrowHelper );
} );

function addShadowedLight( x, y, z, color, intensity ) {

	var directionalLight = new THREE.DirectionalLight( color, intensity );
	directionalLight.position.set( x, y, z );
	scene.add( directionalLight );

	directionalLight.castShadow = true;

	var d = 1;
	directionalLight.shadow.camera.left = -d;
	directionalLight.shadow.camera.right = d;
	directionalLight.shadow.camera.top = d;
	directionalLight.shadow.camera.bottom = -d;

	directionalLight.shadow.camera.near = 1;
	directionalLight.shadow.camera.far = 4;

	directionalLight.shadow.mapSize.width = 1024;
	directionalLight.shadow.mapSize.height = 1024;

	directionalLight.shadow.bias = -0.002;

}

/* global THREE */
scene.add( new THREE.HemisphereLight( 0x443333, 0x111122 ) );

addShadowedLight( 10, 10, 10, 0xffffff, 1.35 );
addShadowedLight( -10, -10, 10, 0xffffff, 0.5 );

let targetQuat = false;

/* global Quaternian, Vector3 */
let setSubmarineVector = (vX, vY, vZ)=>{
  
  // if(vX > 32767){
  //   vX = 32767 - vX
  // }
  // if(vY > 32767){
  //   vY = vY % 32767
  // }
  // if(vZ > 32767){
  //   vZ = 32767 - vZ
  // }
  
  let vector = new THREE.Vector3(vX,vY,vZ);
  vector.normalize(); 
  console.log(vX, vY, vZ)
  if(arrowHelper){
    arrowHelper.setDirection(vector)
  }
  // if(submesh){
  //   submesh.lookAt(vector)
  // }
  // const rotQuat = (new THREE.Quaternion()).setFromAxisAngle(vector, 1)
  // targetQuat = new THREE.Quaternion();
  // targetQuat.multiplyQuaternions(baseQuat,  rotQuat)
};

// Render Loop
var render = function () {
  requestAnimationFrame( render );
  //
  if(targetQuat && submesh){
    //console.log('lerp', targetQuat, submesh.quaternion)
    const newQuat = submesh.quaternion.slerp(targetQuat, 0.1)
    submesh.setRotationFromQuaternion(newQuat);
  }
  
  // Render the scene
  renderer.render(scene, camera);
};

render();
