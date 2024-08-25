/*

Algorytmy sortowania używane są do sortowania trójkątów, które rysowane są na scenie.

Dla urozmaicenia projektu, skorzystałem z biblioteki olcPixelGameEngine i kodu "3D Graphics Part #2" autora Javid9x, 
dostępnego na github: https://github.com/OneLoneCoder/Javidx9/tree/master/ConsoleGameEngine/BiggerProjects/Engine3D. 

*/


#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include "matrix.h"
#include <fstream>
#include <strstream>
#include <algorithm>
#include <chrono>
#include "algorytmy_sortowania.h"
#include "algorytmy_testowanie.h"

using namespace std;

bool test2 {false};

bool bRot {false};

enum class Mode {
    MENU,
	ALGORYTM_A, // Sortowanie bąbelkowe
	ALGORYTM_B, // Sortowanie przez wstawianie
	ALGORYTM_C, // Sortowanie przez kopcowanie
	ALGORYTM_D, // Sortowanie Quicksort
	ALGORYTM_E, // Sortowanie przez scalanie
	ALGORYTM_F, // Sortowanie przez zliczanie
	ALGORYTM_G, // Sortowanie pozycyjne
	ALGORYTM_H, // Sortowanie kubełkowe
	ALGORYTM_TEST // Testowania wydajnosci algorytmów
};

class olcEngine3D : public olc::PixelGameEngine
{
public:
	olcEngine3D()
	{
		sAppName = "Algorytmy sortowania";
	}

private:
	mesh meshCube;
	mat4x4 matProj;	// Matrix that converts from view space to screen space
	vec3d vCamera;	// Location of camera in world space
	vec3d vLookDir;	// Direction vector along the direction camera points
	float fYaw;		// FPS Camera rotation in XZ plane
	float fTheta;	// Spins World transform

	Mode mode = Mode::MENU;
	std::string performanceMessage {"."};	

	std::vector<double> durations;
    std::vector<std::string> names;

public:
	bool OnUserCreate() override
	{
		// Load object file
		meshCube.LoadFromObjectFile("prezent_light.obj");

		// Projection Matrix
		matProj = Matrix_MakeProjection(90.0f, (float)ScreenHeight() / (float)ScreenWidth(), 0.1f, 1000.0f);

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		// Clear Screen
		Clear(olc::BLACK);
        bool quit =  false;

        if (mode == Mode::MENU) {
			DrawString(15, 15, "1. Sortowanie babelkowe", olc::GREEN);
            DrawString(15, 30, "2. Sortowanie przez wstawianie", olc::GREEN);
			DrawString(15, 45, "3. Sortowanie przez kopcowanie", olc::GREEN);
			DrawString(15, 60, "4. Quicksort", olc::GREEN);
			DrawString(15, 75, "5. Sortowanie przez scalanie", olc::GREEN);
			DrawString(15, 90, "6. Sortowanie przez zliczanie", olc::GREEN);
			DrawString(15, 105, "7. Sortowanie pozycyjne", olc::GREEN);
			DrawString(15, 120, "8. Sortowanie kubelkowe", olc::GREEN);

			DrawString(15, 155, "T - Testowania wydajnosci", olc::CYAN);
            DrawString(15, 170, "Q - exit", olc::GREEN);
            
            if (GetKey(olc::Key::K1).bPressed) {
                mode = Mode::ALGORYTM_A;
			} else if (GetKey(olc::Key::K2).bPressed) {
                mode = Mode::ALGORYTM_B;
            } else if (GetKey(olc::Key::K3).bPressed) {
                mode = Mode::ALGORYTM_C;
            } else if (GetKey(olc::Key::K4).bPressed) {
                mode = Mode::ALGORYTM_D;
            } else if (GetKey(olc::Key::K5).bPressed) {
                mode = Mode::ALGORYTM_E;
            } else if (GetKey(olc::Key::K6).bPressed) {
                mode = Mode::ALGORYTM_F;
            } else if (GetKey(olc::Key::K7).bPressed) {
                mode = Mode::ALGORYTM_G;
            } else if (GetKey(olc::Key::K8).bPressed) {
                mode = Mode::ALGORYTM_H;
            }
			
			
			else if (GetKey(olc::Key::T).bPressed) {
				mode = Mode::ALGORYTM_TEST;
            } else if (GetKey(olc::Key::Q).bPressed) {
				quit = true;
            } 
		} else {
			DrawString(15, 15, "ESC - back to main menu", olc::GREEN);
			if (GetKey(olc::Key::ESCAPE).bReleased) {		
				mode = Mode::MENU;
			} 
		}


/*
		Zebrać wyniki do tablicy, a następnie przedstawić je w postaci wykresu słupkowego 
*/
		if ((mode == Mode::ALGORYTM_TEST)) {

			DrawString(15, 15, "ESC - back to main menu", olc::GREEN);
			DrawString(15, 45, "Porownanie wydajnosci roznych algorytmow sortowania", olc::YELLOW);
			DrawString(15, 60, "1. 250 trojkatow", olc::CYAN);
			DrawString(15, 75, "2. 1000 trojkatow", olc::CYAN);
			DrawString(15, 90, "3. 5000 trojkatow", olc::CYAN);

			if (GetKey(olc::Key::K1).bReleased) {
				const int numTriangles = 250; // 
				durations.clear();
				names.clear();

        		std::vector<triangle> triangles;
        		generateRandomTriangles(triangles, numTriangles);

				std::vector<std::pair<std::string, std::function<void(std::vector<triangle>&)>>> sortingAlgorithms = {
					{"Sortowanie babelkowe", testSortowanieBabelkowe},
					{"Sortowanie przez wstawianie", testSortowaniePrzezWstawianie},
					{"Sortowanie przez kopcowanie", testHeapSort},
					{"Quicksort", testQuickSort},
					{"Sortowanie przez scalanie", testMergeSort},
					{"Sortowanie przez zliczanie", testCountingSort},
					{"Sortowanie pozycyjne", testRadixSort},
					{"Sortowanie kubelkowe", testBucketSortTriangles},
					{"std::sort", testStdSort}
				};

				for (auto& algorithm : sortingAlgorithms) {
					durations.push_back(testSortingAlgorithm(algorithm.second, triangles));
					names.push_back(algorithm.first);
				}
			} else if (GetKey(olc::Key::K2).bReleased) {
				const int numTriangles = 1000; // 
				durations.clear();
				names.clear();

        		std::vector<triangle> triangles;
        		generateRandomTriangles(triangles, numTriangles);

				std::vector<std::pair<std::string, std::function<void(std::vector<triangle>&)>>> sortingAlgorithms = {
					{"Sortowanie babelkowe", testSortowanieBabelkowe},
					{"Sortowanie przez wstawianie", testSortowaniePrzezWstawianie},
					{"Sortowanie przez kopcowanie", testHeapSort},
					{"Quicksort", testQuickSort},
					{"Sortowanie przez scalanie", testMergeSort},
					{"Sortowanie przez zliczanie", testCountingSort},
					{"Sortowanie pozycyjne", testRadixSort},
					{"Sortowanie kubelkowe", testBucketSortTriangles},
					{"std::sort", testStdSort}
				};

				for (auto& algorithm : sortingAlgorithms) {
					durations.push_back(testSortingAlgorithm(algorithm.second, triangles));
					names.push_back(algorithm.first);
				}
				
			} else if (GetKey(olc::Key::K3).bReleased) {
				const int numTriangles = 5000; // 
				durations.clear();
				names.clear();
        		std::vector<triangle> triangles;
        		generateRandomTriangles(triangles, numTriangles);

				std::vector<std::pair<std::string, std::function<void(std::vector<triangle>&)>>> sortingAlgorithms = {
					{"Sortowanie babelkowe", testSortowanieBabelkowe},
					{"Sortowanie przez wstawianie", testSortowaniePrzezWstawianie},
					{"Sortowanie przez kopcowanie", testHeapSort},
					{"Quicksort", testQuickSort},
					{"Sortowanie przez scalanie", testMergeSort},
					{"Sortowanie przez zliczanie", testCountingSort},
					{"Sortowanie pozycyjne", testRadixSort},
					{"Sortowanie kubelkowe", testBucketSortTriangles},
					{"std::sort", testStdSort}
				};

				// for (auto& algorithm : sortingAlgorithms) {
				// 	if (algorithm.first.find("Sortowanie przez zliczanie") != std::string::npos) {
				// 		durations.push_back(testSortingAlgorithm(algorithm.second, triangles));
				// 		names.push_back(algorithm.first);
				// 	}
				// }				
				for (auto& algorithm : sortingAlgorithms) {
					durations.push_back(testSortingAlgorithm(algorithm.second, triangles));
					names.push_back(algorithm.first);
				}
			}

			if (durations.size() > 0) {

				const float margin = 50.0f; // Margines od krawędzi okna
				float barWidth = (ScreenWidth() - 2 * margin) / (durations.size() * 2);
				float maxDuration = *std::max_element(durations.begin(), durations.end());
		        float spacing = (ScreenHeight() - 2 * margin) / 0.5 * durations.size(); // Odstęp między opisami

				for (size_t i = 0; i < durations.size(); ++i) {
					float barHeight = (durations[i] / maxDuration) * (ScreenHeight() - 2 * margin) * 0.8f;
					float barX = margin + i * 2 * barWidth;
					float barY = ScreenHeight() - margin - barHeight;

					FillRect(barX, barY, barWidth, barHeight, olc::RED);
					// DrawString(barX, barY - 10, names[i], olc::WHITE);
					// Rysowanie etykiet obok słupków z numeracją
					std::string label = std::to_string(i + 1) + "."  + names[i];
					// Sformatowanie wartości czasowej do jednego miejsca po przecinku
            		std::ostringstream oss;
					// mierzymy czas wykonania sortowania w mikrosekundach, przelicza go na milisekundy i zwraca tę wartość.
					auto duration_ms = durations[i] / 1000.0;
            		oss << std::fixed << std::setprecision(1) << duration_ms;

					DrawString(barX, barY - 10, std::to_string(i + 1), olc::WHITE);
					DrawString(barX, ScreenHeight() - margin/2, oss.str(), olc::WHITE);
					if (i == durations.size()-1) {
						DrawString(barX + barWidth, ScreenHeight() - margin/2, "ms", olc::WHITE);
					}
					float labelY = 2 * margin + i * 15; //spacing;
					DrawString(ScreenWidth()/2, labelY, label, olc::WHITE);					
				}
			} else {
                    uint32_t scale = 2;
					std::string Message {"Wybierz 1, 2.."};
                    // Get the size of the text
                    olc::vi2d textSize = GetTextSize(Message);

                    // Calculate the position to draw the text so that it is centered
                    int textX = ScreenWidth()/2 - (textSize.x * scale) / 2;
                    int textY = ScreenHeight()/2 - (textSize.y * scale) / 2;

                    // Draw the text
                    DrawString(textX, textY, Message, olc::RED, scale);
			}

		} else {

		// Set up "World Tranmsform" though not updating theta 
		// makes this a bit redundant
		mat4x4 matRotY, matRotX;

		if (GetKey(olc::Key::SPACE).bReleased) {
			bRot = !bRot;
		}

		if(bRot) { 
			fTheta += 1.0f * fElapsedTime; // Uncomment to spin me right round baby right round
		}

		matRotY = Matrix_MakeRotationY(fTheta * 0.5f);
		matRotX = Matrix_MakeRotationX(0.0); //			fTheta);

		mat4x4 matTrans;
		matTrans = Matrix_MakeTranslation(0.0f, 0.0f, 5.0f);

		mat4x4 matWorld;
		matWorld = Matrix_MakeIdentity();	// Form World Matrix
		matWorld = Matrix_MultiplyMatrix(matRotY, matRotX); // Transform by rotation
		matWorld = Matrix_MultiplyMatrix(matWorld, matTrans); // Transform by translation

		// Create "Point At" Matrix for camera
		vec3d vUp = { 0,1,0 };
		vec3d vTarget = { 0,0,1 };
		mat4x4 matCameraRot = Matrix_MakeRotationY(fYaw);
		vLookDir = Matrix_MultiplyVector(matCameraRot, vTarget);
		vTarget = Vector_Add(vCamera, vLookDir);
		mat4x4 matCamera = Matrix_PointAt(vCamera, vTarget, vUp);

		// Make view matrix from camera
		mat4x4 matView = Matrix_QuickInverse(matCamera);

		// Store triagles for rastering later
		vector<triangle> vecTrianglesToRaster;

		// Draw Triangles
		for (auto tri : meshCube.tris)
		{
			triangle triProjected, triTransformed, triViewed;

			// World Matrix Transform
			triTransformed.p[0] = Matrix_MultiplyVector(matWorld, tri.p[0]);
			triTransformed.p[1] = Matrix_MultiplyVector(matWorld, tri.p[1]);
			triTransformed.p[2] = Matrix_MultiplyVector(matWorld, tri.p[2]);

			// Calculate triangle Normal
			vec3d normal, line1, line2;

			// Get lines either side of triangle
			line1 = Vector_Sub(triTransformed.p[1], triTransformed.p[0]);
			line2 = Vector_Sub(triTransformed.p[2], triTransformed.p[0]);

			// Take cross product of lines to get normal to triangle surface
			normal = Vector_CrossProduct(line1, line2);

			// You normally need to normalise a normal!
			normal = Vector_Normalise(normal);
			
			// Get Ray from triangle to camera
			vec3d vCameraRay = Vector_Sub(triTransformed.p[0], vCamera);

			// If ray is aligned with normal, then triangle is visible
			if (Vector_DotProduct(normal, vCameraRay) < 0.0f)
			{
				// Illumination
				vec3d light_direction = { 0.0f, 1.0f, -1.0f };
				light_direction = Vector_Normalise(light_direction);

				// How "aligned" are light direction and triangle surface normal?
				float dp = max(0.1f, Vector_DotProduct(light_direction, normal));

				// Choose console colours as required (much easier with RGB)
				// CHAR_INFO c = GetColour(dp);
				// triTransformed.col = c.Attributes;
				// triTransformed.sym = c.Char.UnicodeChar;

				// Convert World Space --> View Space
				triViewed.p[0] = Matrix_MultiplyVector(matView, triTransformed.p[0]);
				triViewed.p[1] = Matrix_MultiplyVector(matView, triTransformed.p[1]);
				triViewed.p[2] = Matrix_MultiplyVector(matView, triTransformed.p[2]);
				// triViewed.sym = triTransformed.sym;
				// triViewed.col = triTransformed.col;

				// Clip Viewed Triangle against near plane, this could form two additional
				// additional triangles. 
				int nClippedTriangles = 0;
				triangle clipped[2];
				nClippedTriangles = Triangle_ClipAgainstPlane({ 0.0f, 0.0f, 0.1f }, { 0.0f, 0.0f, 1.0f }, triViewed, clipped[0], clipped[1]);

				// We may end up with multiple triangles form the clip, so project as
				// required
				for (int n = 0; n < nClippedTriangles; n++)
				{
					// Project triangles from 3D --> 2D
					triProjected.p[0] = Matrix_MultiplyVector(matProj, clipped[n].p[0]);
					triProjected.p[1] = Matrix_MultiplyVector(matProj, clipped[n].p[1]);
					triProjected.p[2] = Matrix_MultiplyVector(matProj, clipped[n].p[2]);
					triProjected.col = clipped[n].col;
					triProjected.sym = clipped[n].sym;

					// Scale into view, we moved the normalising into cartesian space
					// out of the matrix.vector function from the previous videos, so
					// do this manually
					triProjected.p[0] = Vector_Div(triProjected.p[0], triProjected.p[0].w);
					triProjected.p[1] = Vector_Div(triProjected.p[1], triProjected.p[1].w);
					triProjected.p[2] = Vector_Div(triProjected.p[2], triProjected.p[2].w);

					// X/Y are inverted so put them back
					triProjected.p[0].x *= -1.0f;
					triProjected.p[1].x *= -1.0f;
					triProjected.p[2].x *= -1.0f;
					triProjected.p[0].y *= -1.0f;
					triProjected.p[1].y *= -1.0f;
					triProjected.p[2].y *= -1.0f;

					// Offset verts into visible normalised space
					vec3d vOffsetView = { 1,1,0 };
					triProjected.p[0] = Vector_Add(triProjected.p[0], vOffsetView);
					triProjected.p[1] = Vector_Add(triProjected.p[1], vOffsetView);
					triProjected.p[2] = Vector_Add(triProjected.p[2], vOffsetView);
					triProjected.p[0].x *= 0.5f * (float)ScreenWidth();
					triProjected.p[0].y *= 0.5f * (float)ScreenHeight();
					triProjected.p[1].x *= 0.5f * (float)ScreenWidth();
					triProjected.p[1].y *= 0.5f * (float)ScreenHeight();
					triProjected.p[2].x *= 0.5f * (float)ScreenWidth();
					triProjected.p[2].y *= 0.5f * (float)ScreenHeight();

					// Store triangle for sorting
					vecTrianglesToRaster.push_back(triProjected);
				}			
			}
		}

		// Sortujesz trójkąty, różnymi algorytmami sortowania

		if ((mode == Mode::MENU)) {
			using namespace std::chrono;
			const auto start{std::chrono::steady_clock::now()};
			// Sort triangles from back to front
			sort(vecTrianglesToRaster.begin(), vecTrianglesToRaster.end(), [](triangle &t1, triangle &t2)
			{
				float z1 = (t1.p[0].z + t1.p[1].z + t1.p[2].z) / 3.0f;
				float z2 = (t2.p[0].z + t2.p[1].z + t2.p[2].z) / 3.0f;
				return z1 > z2;
			});
			const auto end{std::chrono::steady_clock::now()};
			auto duration = duration_cast<microseconds>(end - start).count();
			// durations.push_back(duration);

			performanceMessage = "Czas: " + std::to_string(duration) + " us /std::sort/";
			DrawString(15, ScreenHeight() - 15, performanceMessage, olc::YELLOW);
		} else if (mode == Mode::ALGORYTM_A) { // Sortowanie bąbelkowe
			using namespace std::chrono;
			const auto start{std::chrono::steady_clock::now()};

			DrawString(15, 30, "Sortowanie babelkowe", olc::YELLOW);		
			SortowanieBabelkowe(vecTrianglesToRaster);

			// if (test2) {
			// 	//SortowanieBabelkowe(vecTrianglesToRaster);
			// 	std::cout << "......" << std::endl;
			// 	for (const auto& triangle : vecTrianglesToRaster) {
			// 		std::cout << " trojkat " << ", z1:" << triangle.p[0].z << ", z2:" << triangle.p[1].z << ", z3:" << triangle.p[2].z << std::endl;
			// 	}
			// 	test2 = false;
			// }

			const auto end{std::chrono::steady_clock::now()};
			auto duration = duration_cast<microseconds>(end - start).count();

			performanceMessage = "Czas: " + std::to_string(duration) + " us";
			DrawString(15, ScreenHeight() - 15, performanceMessage, olc::YELLOW);
		} else if (mode == Mode::ALGORYTM_B) { // Sortowanie przez wstawianie
			using namespace std::chrono;
			const auto start{std::chrono::steady_clock::now()};

			DrawString(15, 30, "Sortowanie przez wstawianie", olc::YELLOW);
			SortowaniePrzezWstawianie(vecTrianglesToRaster);
			
			const auto end{std::chrono::steady_clock::now()};
			auto duration = duration_cast<microseconds>(end - start).count();
			// durations.push_back(duration);

			performanceMessage = "Czas: " + std::to_string(duration) + " us";
			DrawString(15, ScreenHeight() - 15, performanceMessage, olc::YELLOW);
		} else if (mode == Mode::ALGORYTM_C) { // Sortowanie przez kopcowanie
			using namespace std::chrono;
			const auto start{std::chrono::steady_clock::now()};

			DrawString(15, 30, "Sortowanie przez kopcowanie", olc::YELLOW);
			HeapSort(vecTrianglesToRaster);
			
			const auto end{std::chrono::steady_clock::now()};
			auto duration = duration_cast<microseconds>(end - start).count();
			// durations.push_back(duration);

			performanceMessage = "Czas: " + std::to_string(duration) + " us";
			DrawString(15, ScreenHeight() - 15, performanceMessage, olc::YELLOW);
		} else if (mode == Mode::ALGORYTM_D) { // Sortowanie QuickSort
			using namespace std::chrono;
			const auto start{std::chrono::steady_clock::now()};

			DrawString(15, 45, "Sortowanie QuickSort", olc::YELLOW);
			QuickSort(vecTrianglesToRaster, 0, vecTrianglesToRaster.size() - 1);
			
			const auto end{std::chrono::steady_clock::now()};
			auto duration = duration_cast<microseconds>(end - start).count();
			// durations.push_back(duration);

			performanceMessage = "Czas: " + std::to_string(duration) + " us";
			DrawString(15, ScreenHeight() - 15, performanceMessage, olc::YELLOW);
		} else if (mode == Mode::ALGORYTM_E) { // Sortowanie przez scalanie
			using namespace std::chrono;
			const auto start{std::chrono::steady_clock::now()};

			DrawString(15, 45, "Sortowanie przez scalanie", olc::YELLOW);
			MergeSort(vecTrianglesToRaster, 0, vecTrianglesToRaster.size() - 1);
			
			const auto end{std::chrono::steady_clock::now()};
			auto duration = duration_cast<microseconds>(end - start).count();
			// durations.push_back(duration);
			
			performanceMessage = "Czas: " + std::to_string(duration) + " us";
			DrawString(15, ScreenHeight() - 15, performanceMessage, olc::YELLOW);
		} else if (mode == Mode::ALGORYTM_F) { // Sortowanie przez zliczanie
			using namespace std::chrono;
			const auto start{std::chrono::steady_clock::now()};

			DrawString(15, 45, "Sortowanie przez zliczanie", olc::YELLOW);
			CountingSort(vecTrianglesToRaster);
			
			const auto end{std::chrono::steady_clock::now()};
			auto duration = duration_cast<microseconds>(end - start).count();
			// durations.push_back(duration);
			
			performanceMessage = "Czas: " + std::to_string(duration) + " us";
			DrawString(15, ScreenHeight() - 15, performanceMessage, olc::YELLOW);
		} else if (mode == Mode::ALGORYTM_G) { // Sortowanie pozycyjne
			using namespace std::chrono;
			const auto start{std::chrono::steady_clock::now()};

			DrawString(15, 45, "Sortowanie pozycyjne", olc::YELLOW);
			RadixSort(vecTrianglesToRaster);
			
			const auto end{std::chrono::steady_clock::now()};
			auto duration = duration_cast<microseconds>(end - start).count();
			
			performanceMessage = "Czas: " + std::to_string(duration) + " us";
			DrawString(15, ScreenHeight() - 15, performanceMessage, olc::YELLOW);
		} else if (mode == Mode::ALGORYTM_H) { // Sortowanie kubełkowe
			using namespace std::chrono;
			const auto start{std::chrono::steady_clock::now()};

			DrawString(15, 45, "Sortowanie kubelkowe", olc::YELLOW);
			bucketSortTriangles(vecTrianglesToRaster);

			const auto end{std::chrono::steady_clock::now()};
			auto duration = duration_cast<microseconds>(end - start).count();
			
			performanceMessage = "Czas: " + std::to_string(duration) + " us";
			DrawString(15, ScreenHeight() - 15, performanceMessage, olc::YELLOW);
		}

		// Loop through all transformed, viewed, projected, and sorted triangles
		for (auto &triToRaster : vecTrianglesToRaster)
		{
			// Clip triangles against all four screen edges, this could yield
			// a bunch of triangles, so create a queue that we traverse to 
			//  ensure we only test new triangles generated against planes
			triangle clipped[2];
			list<triangle> listTriangles;

			// Add initial triangle
			listTriangles.push_back(triToRaster);
			int nNewTriangles = 1;

			for (int p = 0; p < 4; p++)
			{
				int nTrisToAdd = 0;
				while (nNewTriangles > 0)
				{
					// Take triangle from front of queue
					triangle test = listTriangles.front();
					listTriangles.pop_front();
					nNewTriangles--;

					// Clip it against a plane. We only need to test each 
					// subsequent plane, against subsequent new triangles
					// as all triangles after a plane clip are guaranteed
					// to lie on the inside of the plane. I like how this
					// comment is almost completely and utterly justified
					switch (p)
					{
					case 0:	nTrisToAdd = Triangle_ClipAgainstPlane({ 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, test, clipped[0], clipped[1]); break;
					case 1:	nTrisToAdd = Triangle_ClipAgainstPlane({ 0.0f, (float)ScreenHeight() - 1, 0.0f }, { 0.0f, -1.0f, 0.0f }, test, clipped[0], clipped[1]); break;
					case 2:	nTrisToAdd = Triangle_ClipAgainstPlane({ 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, test, clipped[0], clipped[1]); break;
					case 3:	nTrisToAdd = Triangle_ClipAgainstPlane({ (float)ScreenWidth() - 1, 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f }, test, clipped[0], clipped[1]); break;
					}

					// Clipping may yield a variable number of triangles, so
					// add these new ones to the back of the queue for subsequent
					// clipping against next planes
					for (int w = 0; w < nTrisToAdd; w++)
						listTriangles.push_back(clipped[w]);
				}
				nNewTriangles = listTriangles.size();
			}

			int i=1;
			// Draw the transformed, viewed, clipped, projected, sorted, clipped triangles
			for (auto &triProjected : listTriangles)
			{
				// FillTriangle(t.p[0].x, t.p[0].y, t.p[1].x, t.p[1].y, t.p[2].x, t.p[2].y, t.sym, t.col);
                // FillTriangle(t.p[0].x, t.p[0].y, t.p[1].x, t.p[1].y, t.p[2].x, t.p[2].y);
				// DrawTriangle(t.p[0].x, t.p[0].y, t.p[1].x, t.p[1].y, t.p[2].x, t.p[2].y, olc::GREEN);

				if (i % 2) {
					FillTriangle(triProjected.p[0].x, triProjected.p[0].y,
						triProjected.p[1].x, triProjected.p[1].y,
						triProjected.p[2].x, triProjected.p[2].y, olc::GREEN );
					DrawTriangle(triProjected.p[0].x, triProjected.p[0].y, 
						triProjected.p[1].x, triProjected.p[1].y, 
						triProjected.p[2].x, triProjected.p[2].y, olc::RED);
				} else {
					FillTriangle(triProjected.p[0].x, triProjected.p[0].y,
						triProjected.p[1].x, triProjected.p[1].y,
						triProjected.p[2].x, triProjected.p[2].y, olc::YELLOW );
					DrawTriangle(triProjected.p[0].x, triProjected.p[0].y, 
						triProjected.p[1].x, triProjected.p[1].y, 
						triProjected.p[2].x, triProjected.p[2].y, olc::RED);
				}
				i++;
				
			}
		}

	} // 

	if ((mode != Mode::ALGORYTM_TEST)) {
		DrawString(ScreenWidth()-300, ScreenHeight()-15, "Powered by olcPixelGameEngine, 2024(7)", olc::CYAN );
	}

		return !quit;
	}

};

int main()
{
	olcEngine3D demo;
	if (demo.Construct(512, 480, 2, 2))
		demo.Start();
    return 0;
}

/*

g++ -o start3 zad3.cpp matrix.cpp algorytmy_sortowania.cpp -lX11 -lGL -lpthread -lpng -lstdc++fs -std=c++20^C

*/
