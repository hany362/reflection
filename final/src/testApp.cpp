#include "testApp.h"
using namespace ofxCv;
using namespace cv;

//--------------------------------------------------------------
void testApp::setup(){
    ofSetVerticalSync(true);
	//ofSetFrameRate(60);
	ofSetFrameRate(30);
    
    cam.initGrabber(640,480);
    
    ofSetBackgroundAuto(FALSE);
    ofEnableAlphaBlending();
    //ofBackground(0, 0, 0);
    ofBackground(255, 255, 255);

    
// // // // // // // // // // // // // // // // // // //
    fbo.allocate(512, 512, GL_RGBA);
// // // // // // // // // // // // // // // // // // //     
    
    // MESSAGES OF STRINGS
    
    counter=0;
    bCounted=true;

// // // // // // // // // // // // // // // // // // //
    
//  // // // // // // // // // // // // // // // // 
    
    
#ifdef TARGET_OPENGLES
	shader.load("shadersES2/shader");
#else
	if(ofIsGLProgrammableRenderer()){
		shader.load("shadersGL3/shader");
	}else{
		shader.load("shadersGL2/shader");
	}
#endif
    
    //imageMask.loadImage("trial03.png");//original one
    //imageMask.loadImage("trial06.png");
    //imageMask.loadImage("img_mask04.png"); //original one
    image.loadImage("image_mask.png");
    //image.loadImage("img_mask03.png");
    imageMask.loadImage("img_mask03.png");
    
    
    mesh.setMode(OF_PRIMITIVE_TRIANGLES);
    //ofSetLineWidth(300*xSteps*ySteps*stepSize);
    //ofSetLineWidth(10);
    //mesh.setMode(OF_PRIMITIVE_LINES);
    //mesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
	//mesh.setMode(OF_PRIMITIVE_LINE_STRIP);
    
        
    stepSize = 6;
	ySteps = cam.getHeight() / stepSize;
	xSteps = cam.getWidth() / stepSize;
	for(int y = 0; y < ySteps; y++) {
		for(int x = 0; x < xSteps; x++) {
			mesh.addVertex(ofVec2f(x * stepSize, y * stepSize));
			mesh.addTexCoord(ofVec2f(x * stepSize, y * stepSize));
            
		}
	}
	for(int y = 0; y + 1 < ySteps; y++) {
		for(int x = 0; x + 1 < xSteps; x++) {
			int nw = y * xSteps + x;
			int ne = nw + 1;
			int sw = nw + xSteps;
			int se = sw + 1;
			mesh.addIndex(nw);
			mesh.addIndex(ne);
			mesh.addIndex(se);
			mesh.addIndex(nw);
			mesh.addIndex(se);
			mesh.addIndex(sw);
		}
	}
    
    
}


//--------------------------------------------------------------
void testApp::update(){
    cam.update();
    
    if(cam.isFrameNew()) {
        flow.setWindowSize(8);
        flow.calcOpticalFlow(cam);
        int i = 0;
		float distortionStrength = 4;

		for(int y = 1; y + 1 < ySteps; y++) {
			for(int x = 1; x + 1 < xSteps; x++) {
				int i = y * xSteps + x;
                ofVec2f position(x * stepSize, y * stepSize);
				ofRectangle area(position - ofVec2f(stepSize, stepSize) / 0.9, stepSize, stepSize);
				ofVec2f offset = flow.getAverageFlowInRegion(area);
                mesh.setVertex(i, position + distortionStrength * offset);
				i++;

			}
		}
    }
        cout << ofGetElapsedTimef() << endl;

    } 
    
    // // // // // // // // // // // //



//}

//--------------------------------------------------------------
void testApp::draw(){

    ofSetRectMode(OF_RECTMODE_CORNER);
    //ofColor semiTransparent(0,0,0,255.0 *0.05);
    ofColor semiTransparent(0, 0, 0, 255.0 * 0.005);
    ofFill();
    ofSetColor(semiTransparent);
    //ofTriangle(0, 0, 0, image.getWidth(), image.getHeight(), image.getWidth());
    
    glTranslated(ofGetWindowWidth(), 0, 0);
    glScaled(-1, 1, 1);
    
    ofRect(-10, -10, ofGetWindowWidth(), ofGetWindowHeight());
    
	//ofBackground(0);
	ofScale(2.1, 2.1);
    
    //ofRect(0, 0, image.getWidth(), image.getHeight());
    //ofTriangle(0, 0, 0, image.getWidth(), image.getHeight(), image.getWidth());
    
    shader.begin();
    shader.setUniformTexture("imageMask", imageMask.getTextureReference(), 2.5);
    
    image.draw(0, 0);
    imageMask.draw(0,0);
    
	cam.getTextureReference().bind();
    mesh.draw();
    cam.getTextureReference().unbind();
    
    
    cam.draw(500,500,320,240);

    shader.end();

    

//	if(ofGetKeyPressed()) {
//        mesh.drawWireframe();
//
//	}
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    
    if ( key == ' '){
        
        takePhoto.grabScreen(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
        takePhoto.saveImage("Reflection-" +ofToString(snapCounter) + ".png");
        snapCounter++;
    }

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}
