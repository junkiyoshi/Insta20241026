#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(25);
	ofSetWindowTitle("openFrameworks");

	ofBackground(39);
	ofSetLineWidth(3);
	ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_ADD);
	ofEnableDepthTest();

	this->line.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
}

//--------------------------------------------------------------
void ofApp::update() {

	this->noise_param += 0.05;

	this->face.clear();
	this->line.clear();

	float deg_span = 0.2;
	float z_span = 1;

	float radius = 300;
	float noise_span = 0.0025;

	ofColor color;
	for (float i = 0.1; i < 1; i += 0.1) {

		float threshold_1 = i - 0.0025;
		float threshold_2 = i + 0.0025;

		int fue = (int)ofMap(i, 0, 1, 180, 265);
		color.setHsb(fue % 255, 255, 255);

		for (float deg = 0; deg < 360; deg += deg_span) {

			for (float z = -2400; z < 600; z += z_span) {

				auto noise_value = ofNoise(glm::vec4(radius * cos(deg * DEG_TO_RAD) * noise_span, radius * sin(deg * DEG_TO_RAD) * noise_span, z * noise_span * 0.2 - noise_param, noise_param * 0.1));
				if (noise_value <= threshold_1 || noise_value >= threshold_2) { continue; }

				auto noise_1 = ofNoise(glm::vec4(radius * cos((deg - deg_span) * DEG_TO_RAD) * noise_span, radius * sin((deg - deg_span) * DEG_TO_RAD) * noise_span, z * noise_span * 0.2 - noise_param, noise_param * 0.1));
				auto noise_2 = ofNoise(glm::vec4(radius * cos(deg * DEG_TO_RAD) * noise_span, radius * sin(deg * DEG_TO_RAD) * noise_span, (z + z_span) * noise_span * 0.2 - noise_param, noise_param * 0.1));
				auto noise_3 = ofNoise(glm::vec4(radius * cos(deg * DEG_TO_RAD) * noise_span, radius * sin(deg * DEG_TO_RAD) * noise_span, (z - z_span) * noise_span * 0.2 - noise_param, noise_param * 0.1));
				auto noise_4 = ofNoise(glm::vec4(radius * cos((deg + deg_span) * DEG_TO_RAD) * noise_span, radius * sin((deg + deg_span) * DEG_TO_RAD) * noise_span, z * noise_span * 0.2 - noise_param, noise_param * 0.1));

				auto index = this->face.getNumVertices();
				vector<glm::vec3> vertices;

				vertices.push_back(glm::vec3(radius * cos((deg - deg_span * 0.5) * DEG_TO_RAD), radius * sin((deg - deg_span * 0.5) * DEG_TO_RAD), z - z_span * 0.5));
				vertices.push_back(glm::vec3(radius * cos((deg + deg_span * 0.5) * DEG_TO_RAD), radius * sin((deg + deg_span * 0.5) * DEG_TO_RAD), z - z_span * 0.5));
				vertices.push_back(glm::vec3(radius * cos((deg - deg_span * 0.5) * DEG_TO_RAD), radius * sin((deg - deg_span * 0.5) * DEG_TO_RAD), z + z_span * 0.5));
				vertices.push_back(glm::vec3(radius * cos((deg + deg_span * 0.5) * DEG_TO_RAD), radius * sin((deg + deg_span * 0.5) * DEG_TO_RAD), z + z_span * 0.5));

				this->face.addVertices(vertices);

				this->face.addIndex(index + 0); this->face.addIndex(index + 1); this->face.addIndex(index + 3);
				this->face.addIndex(index + 0); this->face.addIndex(index + 2); this->face.addIndex(index + 3);

				for (int i = 0; i < 4; i++) {

					this->face.addColor(ofColor(color, 68));
				}

				ofColor line_color(color);
				if (noise_1 <= threshold_1 || noise_1 >= threshold_2) {

					this->line.addVertex(vertices[0]);
					this->line.addVertex(vertices[2]);

					this->line.addIndex(this->line.getNumVertices() - 1);
					this->line.addIndex(this->line.getNumVertices() - 2);

					this->line.addColor(line_color);
					this->line.addColor(line_color);
				}

				if (noise_2 <= threshold_1 || noise_2 >= threshold_2) {

					this->line.addVertex(vertices[2]);
					this->line.addVertex(vertices[3]);

					this->line.addIndex(this->line.getNumVertices() - 1);
					this->line.addIndex(this->line.getNumVertices() - 2);

					this->line.addColor(line_color);
					this->line.addColor(line_color);
				}

				if (noise_3 <= threshold_1 || noise_3 >= threshold_2) {

					this->line.addVertex(vertices[0]);
					this->line.addVertex(vertices[1]);

					this->line.addIndex(this->line.getNumVertices() - 1);
					this->line.addIndex(this->line.getNumVertices() - 2);

					this->line.addColor(line_color);
					this->line.addColor(line_color);
				}

				if (noise_4 <= threshold_1 || noise_4 >= threshold_2) {

					this->line.addVertex(vertices[1]);
					this->line.addVertex(vertices[3]);

					this->line.addIndex(this->line.getNumVertices() - 1);
					this->line.addIndex(this->line.getNumVertices() - 2);

					this->line.addColor(line_color);
					this->line.addColor(line_color);
				}
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();

	this->line.draw();
	this->face.draw();

	ofSetColor(0);
	ofFill();
	ofDrawCircle(glm::vec3(0, 0, -2500), 300);

	ofColor color;
	color.setHsb(225, 200, 255);
	ofSetColor(color);
	ofNoFill();
	ofDrawCircle(glm::vec3(0, 0, -2500), 300);

	this->cam.end();

	/*
	int start = 1;
	if (ofGetFrameNum() > start) {

		ostringstream os;
		os << setw(4) << setfill('0') << ofGetFrameNum() - start;
		ofImage image;
		image.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
		image.saveImage("image/cap/img_" + os.str() + ".jpg");
		if (ofGetFrameNum() - start >= 25 * 20) {

			std::exit(1);
		}
	}
	*/
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}