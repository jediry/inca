#define GL_HPP_IMPORT_GLUT 1
#include <inca/integration/opengl/GL.hpp>
#include <inca/ui.hpp>
using namespace inca::ui;

#include <inca/math/linalg.hpp>
using namespace inca::math;


#include <inca/raster/MultiArrayRaster>
#include <inca/raster/operators/arithmetic>
#include <inca/raster/operators/clamp>
#include <inca/raster/operators/select>
#include <inca/raster/operators/linear_map>
#include <inca/raster/operators/fourier>
#include <inca/raster/operators/magnitude>
#include <inca/raster/operators/rotate>
#include <inca/raster/operators/gradient>
#include <inca/raster/generators/gaussian>
#include <inca/raster/generators/linear_fade>
#include <inca/raster/algorithms/scale_space_project>
#include <inca/raster/algorithms/find_edges>
using namespace inca::raster;

using namespace std;

#include <inca/util/Timer>

#include <inca/util/metaprogramming/macros.hpp>
#include <inca/util/multi-dimensional-macros.hpp>


typedef MultiArrayRaster<float, 2> Image;
typedef MultiArrayRaster<float, 3> HyperImage;
typedef MultiArrayRaster< std::complex<float>, 2> ComplexImage;

typedef inca::Timer<float> Timer;

#define GUI_TOOLKIT GLUT

// Simple window displaying an image
class ImageWindow : public WINDOW(GUI_TOOLKIT) {
public:
    // Constructor taking an image
    template <class ImageType>
    ImageWindow(const ImageType &img,
                const std::string &title = "Real Image")
            : WINDOW(GUI_TOOLKIT)(title),
              image(inca::FortranStorageOrder()),
              hyperImage(inca::FortranStorageOrder()),
              complexImage(inca::FortranStorageOrder()) {

        // Setup the image
        loadImage(img);
    }

    // Setter for a real, 2D image
    template <class ImageType>
    void loadImage(const ImageType & img,
            ENABLE_FUNCTION_IF( AND2( EQUAL( INT(ImageType::dimensionality), INT(2)),
                            IS_SAME(typename ImageType::ElementType, float )))) {
        image = linear_map(img, inca::Array<float, 2>(0.0f, 1.0f));
//        image = img;
        Window::setSize(image.size(0), image.size(1));
        imageMode = 0;
    }

    // Setter for a real, 3D image
    template <class ImageType>
    void loadImage(const ImageType & img,
            ENABLE_FUNCTION_IF( EQUAL( INT(ImageType::dimensionality), INT(3)))) {
        cerr << "Setting hyper image: \n";
        hyperImage = linear_map(img, inca::Array<float, 2>(0.0f, 1.0f));
//        hyperImage = img;
        Window::setSize(hyperImage.size(0), hyperImage.size(1));
        imageMode = 1;
        layer = 0;
    }

    template <class ImageType>
    void loadImage(const ImageType & img,
            ENABLE_FUNCTION_IF( IS_SAME( typename ImageType::ElementType,
                                         std::complex<float> ))) {
        cerr << "Setting complex image: \n";
        complexImage = img;
        Window::setSize(complexImage.size(0) * 2, complexImage.size(1));
        imageMode = 2;
    }

    void reshape(int width, int height) {
        GLUTWindow::reshape(width, height);
        GL::glMatrixMode(GL_PROJECTION);
        GL::glLoadIdentity();
        GL::glMultMatrix(inca::math::screenspaceLLMatrix<double, false, false>(getSize()).elements());
        GL::glMatrixMode(GL_MODELVIEW);
    }

    void display() {
        GL::glClearColor(0.5f, 0.5f, 0.5f, 0.0f);
        GL::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        GL::glRasterPos2d(0.0, 0.0);

        switch (imageMode) {
        case 0:     // Real, 2D
            GL::glDrawPixels(image.size(0), image.size(1),
                             GL_LUMINANCE, GL_FLOAT, image.elements());
            break;
        case 1:     // Real, 3D
            GL::glDrawPixels(hyperImage.size(0), hyperImage.size(1),
                             GL_LUMINANCE, GL_FLOAT, hyperImage.elements()
                 + hyperImage.array().indexOf(inca::Array<int, 3>(0, 0, layer)));
            break;
        case 2:     // Complex, 2D
            GL::glDrawPixels(complexImage.size(0) * 2, complexImage.size(1),
                             GL_LUMINANCE, GL_FLOAT, complexImage.elements());
            break;
        }
        GL::glutSwapBuffers();
    }

    void mouseButton(int button, int state, int x, int y) {
        y = this->getSize()[1] - y;

        if (state == GLUT_DOWN) {
            switch (imageMode) {
            case 0:     // Real, 2D
                image(x, y) = 1.0f;
                break;
            case 1:     // Real, 3D
                if (button == GLUT_LEFT_BUTTON && layer < hyperImage.size(2) - 1)
                    ++layer;
                if (button == GLUT_RIGHT_BUTTON && layer > 0)
                    --layer;
                cerr << "Displaying scale layer " << layer << endl;
                break;
            case 2:     // Complex, 2D
                complexImage(x/2, y) = std::complex<float>(1.0f, 1.0f);
                break;
            }
            requestRedisplay();
        }
    }


protected:
    int             imageMode, layer;
    Image           image;
    HyperImage      hyperImage;
    ComplexImage    complexImage;
};
        // Clamp the values in a raster to a certain range
        INCA_RASTER_OPERATOR_CLASS_HEADER(AndOp,
                                          2, NIL,
                                          typename R0::ElementType ) {
        public:
            // Get types from the superclass
            INCA_RASTER_OPERATOR_IMPORT_TYPES(AndOp<R0 COMMA R1>)

            // Constructor giving clamp-to range
            explicit AndOp(const R0 & r0, const R1 & r1)
                    : OperatorBaseType(r0, r1) { }

        protected:
            // Element evaluator function
            INCA_RASTER_OPERATOR_GET_ELEMENT_HEADER(indices) {
                return (std::abs(operand0(indices)) < 0.001f && operand1(indices) < -0.0000001f)
                    ? 1.0f : 0.0f;
            }
        };

        // Clamp the values in a raster to a certain range
        INCA_RASTER_OPERATOR_CLASS_HEADER(ZeroCross,
                                          1, NIL,
                                          typename R0::ElementType ) {
        public:
            // Get types from the superclass
            INCA_RASTER_OPERATOR_IMPORT_TYPES(ZeroCross<R0>)

            // Constructor giving clamp-to range
            explicit ZeroCross(const R0 & r0)
                    : OperatorBaseType(r0) { }

        protected:
            // Element evaluator function
            INCA_RASTER_OPERATOR_GET_ELEMENT_HEADER(indices) {
                bool hit = false;
                ElementType e = operand0(indices);
                IndexArray idx(indices);
                for (IndexType d = 0; d < dimensionality; ++d) {
                    idx[d]--;
                    if (operand0(idx) * e < 0)      hit = true;
                    idx[d] += 2;
                    if (operand0(idx) * e < 0)      hit = true;
                    idx[d]--;
                }
                return hit ? 1.0f : 0.0f;
            }
        };

class RasterTest : public GLUTApplication {
    void constructInterface() {
        typedef inca::Array<inca::IndexType, 2> IndexArray;

        Image original(200, 200);
        fill(original, 0.5f);
//        select(original, IndexArray(50, 50)) = select(constant<float, 2>(1.0f), IndexArray(100, 100));
        select(original, IndexArray(25, 25), IndexArray(50, 50))
            = select(constant<float, 2>(1.0f), IndexArray(50, 50));

        WindowPtr win1(new ImageWindow(original));
        registerWindow(win1);

        HyperImage scaleSpace;
        scaleSpace.setOutOfBoundsPolicy(Nearest);
        inca::Array<float, 6> scales(0.0f, 1.0f, 2.0f, 4.0f, 8.0f, 16.0f);
//        inca::Array<float, 5> scales(10.0f, 20.0f, 40.0f, 80.0f, 160.0f);
        scale_space_project(scaleSpace, original, scales);
        WindowPtr win3(new ImageWindow(scaleSpace));
        registerWindow(win3);

//         inca::Array<float, 100> testArray;
//         for (int i = 0; i < 100; ++i)
//             testArray[i] = float(i) / 100;
//         HyperImage img4 = select(array<inca::Array<float, 100>, 3>(testArray, 2), inca::Array<int, 3>(100,100, 100));
//         WindowPtr win4(new ImageWindow(img4));
//         registerWindow(win4);


#if 0
        Timer timer;
        timer.start();
        HyperImage edgeSurface(scaleSpace.sizes()), edgeSign(scaleSpace.sizes()),
                    strengthSurface(scaleSpace.sizes()), strengthSign(scaleSpace.sizes());
        find_edges(scaleSpace, scales, edgeSurface, edgeSign, strengthSurface, strengthSign);
        timer.update();
        timer.stop();
        cerr << "Finding edges took " << timer() << " seconds" << endl;

        WindowPtr win4(new ImageWindow(edgeSurface, "Edge Surface"));
        registerWindow(win4);
#if 0
        WindowPtr win5(new ImageWindow(edgeSign, "Edge Sign"));
        registerWindow(win5);
        WindowPtr win6(new ImageWindow(strengthSurface, "Strength Surface"));
        registerWindow(win6);
        WindowPtr win7(new ImageWindow(strengthSign, "Strength Sign"));
        registerWindow(win7);

        WindowPtr winA(new ImageWindow(ZeroCross<HyperImage>(edgeSurface), "Edge Zeros"));
        registerWindow(winA);
        WindowPtr winB(new ImageWindow(min(edgeSign, constant<float, 3>(-0.0000001f)), "Edge Neg Sign"));
        registerWindow(winB);

        HyperImage eComp = AndOp<HyperImage, HyperImage>(edgeSurface, edgeSign);
        WindowPtr win8(new ImageWindow(eComp, "Edge Composite"));
        registerWindow(win8);

        HyperImage sComp = AndOp<HyperImage, HyperImage>(strengthSurface, strengthSign);
        WindowPtr win9(new ImageWindow(sComp, "Strength Composite"));
        registerWindow(win9);
#endif
#endif
#if 0
        Image img2(100, 100);
        fill(img2, 0.5f);

//        select(img1, IndexArray(50, 50)) -= select(img2);
        Image img3 = img2 / 5;
        img3 = img1 * (gaussian<float, 2>(inca::Array<float, 2>(100.0f), inca::Array<float, 2>(50.0f)) * 100000.0f);
//        WindowPtr win(new ImageWindow(img3));

        select(img1, IndexArray(50, 50), IndexArray(100, 100)) = select(constant<float, 2>(1.0f), img1.bounds());

        MultiArrayRaster<std::complex<float>, 2> myDFT = dft(img3);
//        select(myDFT, IndexArray(0, 0), IndexArray(200, 200)) = select(constant< std::complex<float>, 2>(std::complex<float>(1.0f, 1.0f)), myDFT.bounds());

//        for (int i = 0; i < 100; ++i)
//            myDFT(50 + i, 0 + i) = std::complex<float>(1.0f, 1.0f);
//        for (int i = 0; i < 100; ++i)
//            for (int j = -10; j < 10; ++j)
//                myDFT(50 + j, i) = std::complex<float>(1.0f, 1.0f);
//cerr << "Total # elements " << myDFT.size() << endl;
//        cerr << "Value at (150, 50) " << myDFT(150, 50) << endl;
//        for (int i = 0; i < myDFT.size(); ++i)
//            const_cast<inca::MultiArray<std::complex<float>, 2> &>(myDFT.array())[i] = std::complex<float>(1.0f, 0.0f);
        WindowPtr win2(new ImageWindow(abs(img3 - idft(dft(img3)))));
        registerWindow(win2);

//        select(img3) += img2;
#endif

//        Image img4 = cmagnitude(myDFT);
//        cerr << "Size of dft is " << dft(img3).sizes() << endl;
//        WindowPtr win(new ImageWindow(cmagnitude(dft(img1))));
//        WindowPtr win(new ImageWindow(img1));
//        registerWindow(win);

    }
};


APPLICATION_MAIN(RasterTest);
