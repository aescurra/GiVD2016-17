#include <math.h>
#include <GLWidget.h>
#include <QtDebug>
#include <QString>


GLWidget::GLWidget(QWidget *parent) : QGLWidget(QGLFormat(QGL::SampleBuffers), parent) {
    setFocusPolicy( Qt::StrongFocus );
    scene = new Scene();
    camera = new Camera();
}

GLWidget::~GLWidget() {
    makeCurrent();
    delete scene;
}

void GLWidget::activaAlpha1() {
    cout<<"Material: alpha 1"<<endl;
    scene->setAlphaObject(1.0);
    updateGL();
}
void GLWidget::activaAlpha075() {
    cout<<"Material: alpha 0.50"<<endl;
    scene->setAlphaObject(0.50);
    updateGL();
}
void GLWidget::activaAlpha050() {
    cout<<"Material: alpha 0.25"<<endl;
    scene->setAlphaObject(0.25);
    updateGL();
}

// Metodes que es criden des dels menús

void GLWidget::activaToonShader() {
    //A implementar a la fase 1 de la practica 2
    cout<<"Estic a Toon"<<endl;

    updateShader("://resources/vshadertoon.glsl", "://resources/fshadertoon.glsl");


}

void GLWidget::activaPhongShader() {
    //A implementar a la fase 1 de la practica 2
    cout<<"Estic a Phong"<<endl;
    updateShader("://resources/vshaderphong.glsl", "://resources/fshaderphong.glsl");
    //program->link();
    //program->bind();

    //updateGL();

}

void GLWidget::activaGouraudShader() {
    //A implementar a la fase 1 de la practica 2
    cout<<"Estic a Gouraud"<<endl;
    updateShader("://resources/vshadergouraud.glsl", "://resources/fshadergouraud.glsl");
    //program->link();
    //program->bind();

    //updateGL();

}

void GLWidget::activaPhongTex() {
    //A implementar a la fase 1 de la practica 2
    cout<<"Estic a Phong Tex"<<endl;
    updateShaderTexture("://resources/vshaderphong.glsl", "://resources/fshaderphongTexture.glsl");

}

void GLWidget::activaGouraudTex() {
    //A implementar a la fase 1 de la practica 2
    //scene->drawTexture();
    updateShaderTexture("://resources/vshadergouraud.glsl", "://resources/fshadergouraudTexture.glsl");
    //this->updateShaderTexture();
}

//Metode  per canviar de shaders.
void GLWidget::updateShader(const char* vertexShaderFile, const char* fragmentShaderFile){
    //A implementar a la fase 1 de la practica 2
    //program->removeAllShaders();
    QGLShader *vshader = new QGLShader(QGLShader::Vertex, this);
    QGLShader *fshader = new QGLShader(QGLShader::Fragment, this);

    vshader->compileSourceFile(vertexShaderFile);
    fshader->compileSourceFile(fragmentShaderFile);

    program = new QGLShaderProgram(this);
    program->removeAllShaders();
    program->addShader(vshader);
    program->addShader(fshader);
    program->link();
    program->bind();

    updateGL();

}

//Metode per canviar de shaders de textures
void GLWidget::updateShaderTexture(const char* vertexShaderFile, const char* fragmentShaderFile){
    //A implementar a la fase 1 de la practica 2
    program = new QGLShaderProgram(this);

    scene->drawTexture();//aqui llamamos a este metodo para inicializar las teturas y luego para cada objeto
    //hacer el draw

    QGLShader *vshader = new QGLShader(QGLShader::Vertex, this);
    QGLShader *fshader = new QGLShader(QGLShader::Fragment, this);

    vshader->compileSourceFile(vertexShaderFile);
    fshader->compileSourceFile(fragmentShaderFile);
    program->removeAllShaders();


    program->addShader(vshader);
    program->addShader(fshader);
    program->link();
    program->bind();

    updateGL();

}
void GLWidget::ensenyaMenuLight0() {
    // Agafa la darrera llum creada. A CANVIAR si es vol tenir control de totes les llums.
    // De fet el metode showAuxWindowPuntualLight té com a parametre la llum a mostrar en el widget.
    // Es podria cridar diverses vegades.
    if (scene->lights.size()>0)
        showAuxWindowPuntualLight(scene->getLightActual(this->typeL));//default puntual light
}

// Per a les llums: com afegir-les
void GLWidget::changePositionLight() {
    // TO DO: cal modificar en la fase 1 de la practica 2
    // Des d'quest mètode s'AFEGEIX una nova llum
    // tipus rep el tipus de llum que es vol afegir. Des d'aqui s'afegeix la llum a l'escena
    this->typeL = 0;
    scene->getLightActual(this->typeL)->setTipusLight(Puntual);
    scene->lightsToGPU(program);
    updateGL();

    cout << "Change light to Puntual" << endl;

}
void GLWidget::changeDirectionalLight() {
    // tipus rep el tipus de llum que es vol afegir. Des d'aqui s'afegeix la llum a l'escena.
    // TO DO: cal modificar en la fase 1 de la practica 2
    // Des d'quest mètode s'AFEGEIX una nova llum
    //scene->getLightActual(1)->setTipusLight(Direccional);

    this->typeL = 1;
    Light *direccional = new Light(Direccional);
    direccional->setDiffuseIntensity(vec3(1.0,0.5,0.5));
    direccional->setIa(vec3(0.2,0.2,0.2));
    direccional->setIs(vec3(1.0,1.0,1.0));
    direccional->setLightPosition(vec4(4.0,4.0,4.0,1.0));
    direccional->setDirection(vec4(4.0,4.0,4.0,1.0));
    direccional->setCoeficients(vec3(0.0,0.0,0.5));
    direccional->setEstaActivat(true);

    scene->addLight(direccional);
    scene->lightsToGPU(program);
    updateGL();

    cout << "Change light to Direccional" << endl;

}
void GLWidget::changeSpotLight() {
    // tipus rep el tipus de llum que es vol afegir. Des d'aqui s'afegeix la llum a l'escena.
    // TO DO: cal modificar en la fase 1 de la practica 2
    // Des d'quest mètode s'AFEGEIX una nova llum
    scene->getLightActual(2)->setTipusLight(Spot);
    updateGL();

}
void GLWidget::updateXPositionLight(int xposition) {
    // S'ha de modificar la posicio x de la llum activa
    vec4 v = scene->getLightActual(this->typeL)->getLightPosition();
    v[0] = (float)xposition;
    scene->getLightActual(this->typeL)->setLightPosition(v);
    scene->lightsToGPU(program);
    updateGL();
}

void GLWidget::updateYPositionLight(int yposition) {
    // S'ha de modificar la posicio y de la llum activa
    vec4 v = scene->getLightActual(this->typeL)->getLightPosition();
    v[1] = (float)yposition;
    scene->getLightActual(this->typeL)->setLightPosition(v);
    scene->lightsToGPU(program);
    updateGL();
}

void GLWidget::updateZPositionLight(int zposition) {
    // S'ha de modificar la posicio z de la llum activa
    vec4 v = scene->getLightActual(this->typeL)->getLightPosition();
    v[2] = (float)zposition;
    scene->getLightActual(this->typeL)->setLightPosition(v);
    scene->lightsToGPU(program);
    updateGL();
}

void GLWidget::updateLightIntensity(int intens) {
    // S'ha de modificar la intensitat de la llum 0. es podria canviar per la llum actual
    vec3 intensitat;
    intensitat =  scene->getLightActual(this->typeL)->getDiffuseIntensity();
    intensitat[0] = intens/200.0;
    intensitat[1] = intens/200.0;
    intensitat[2] = intens/200.0; // el 200 es l'escala del scrollbar

    scene->getLightActual(this->typeL)->setDiffuseIntensity(intensitat);
    scene->lightsToGPU(program);
    updateGL();


}

void GLWidget::activateLight(){
    scene->getLightActual(this->typeL)->switchOnOff();
    scene->lightsToGPU(program);
    updateGL();
}

void GLWidget::activaBumpMapping() {
    //TO DO: a implementar a la fase 2 de la practica 2
}

void GLWidget::activaEnvMapping() {
    //To DO: a implementar a la fase 2 de la practica 2
}



// Mètodes d'interacció amb el ratolí

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
    std::cout<<"Press button"<<std::endl;
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();
        std::cout<<"M button"<<std::endl;
    if (event->buttons() & Qt::LeftButton)
    {
        if(lastPos.y()!= event->y() && lastPos.x()!= event->x()) {
           setXRotation(dy);
           setYRotation(dx);
        } else if(lastPos.y()!= event->y()) {// rotar la camera
            setXRotation(dy);
        } else if (lastPos.x()!= event->x()) {
            setYRotation(dx);
        }

    } else if (event->buttons() & Qt::RightButton) {
       // Zoom: canviar la mida de la window
        if(lastPos.y()> event->y())
             Zoom(-1);
        else
             Zoom(1);
    }

    lastPos = event->pos();
    camera->toGPU(program);
    updateGL();
}


void GLWidget::setXRotation(int angle)
{
    if (angle >0) {
        camera->angX += 5;
    } else if (angle<0)
        camera->angX -= 5;
    qNormalizeAngle(camera->angX);

    camera->rotaCamera(scene->capsaMinima);
    updateGL();
}

void GLWidget::setYRotation(int angle)
{
    if (angle >0) {
        camera->angY += 5;
    } else if (angle<0)
        camera->angY-= 5;
    qNormalizeAngle(camera->angY);

    camera->rotaCamera(scene->capsaMinima);
    updateGL();

}

void GLWidget::qNormalizeAngle(double &angle)
{
    while (angle < 0)
        angle += 360*16;
    while (angle > 360)
        angle -= 360*16;
}

void GLWidget::Zoom (int positiu) {
    camera->AmpliaWindow(positiu*(0.005));

    updateGL();

}

void GLWidget::initShader(const char* vShaderFile, const char* fShaderFile){
    QGLShader *vshader = new QGLShader(QGLShader::Vertex, this);
    QGLShader *fshader = new QGLShader(QGLShader::Fragment, this);

    vshader->compileSourceFile(vShaderFile);
    fshader->compileSourceFile(fShaderFile);

    program = new QGLShaderProgram(this);
    program->addShader(vshader);
    program->addShader(fshader);
    program->link();
    program->bind();
}

/**
 * @brief GLWidget::initShadersGPU
 */
void GLWidget::initShadersGPU(){
    //inicializamos los shaders con GOURAUD
    initShader("://resources/vshadergouraud.glsl", "://resources/fshadergouraud.glsl");
    cout << "first GOURAUD" <<endl;
}

QSize GLWidget::minimumSizeHint() const {
    return QSize(50, 50);
}

QSize GLWidget::sizeHint() const {
    return QSize(400, 400);
}

void GLWidget::initializeGL() {
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);
    //enable trasparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_RGBA);
    glEnable(GL_DOUBLE);

    initShadersGPU();

    // Creacio d'una Light per apoder modificar el seus valors amb la interficie
    Light *puntual = new Light(Puntual);
    puntual->setDiffuseIntensity(vec3(1.0,0.5,0.5));
    puntual->setIa(vec3(0.4,0.4,0.4));
    puntual->setIs(vec3(1.0,1.0,1.0));
    puntual->setLightPosition(vec4(2.0,2.0,2.0,1.0));
    puntual->setCoeficients(vec3(0.0,0.0,0.8));
    puntual->setEstaActivat(true);
    scene->addLight(puntual);

    //cout << "first initGL" <<endl;


    camera->init(this->size().width(), this->size().height(), scene->capsaMinima);
    glViewport(camera->vp.pmin[0], camera->vp.pmin[1], camera->vp.a, camera->vp.h);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GLWidget::paintGL() {
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    camera->toGPU(program);
    this->scene->lightsToGPU(program);
    this->scene->setAmbientToGPU(program);
    scene->draw();

}


void GLWidget::resizeGL(int width, int height) {
    int side = qMin(width, height);
    glViewport((width - side) / 2, (height - side) / 2, side, side);

    camera->vp.pmin[0] = (width - side) / 2;
    camera->vp.pmin[1] = (height - side) / 2;
    camera->vp.a = side;
    camera->vp.h = side;
    updateGL();
}

void GLWidget::newObj(QString fichero){
    qDebug() << fichero;
    Object * obj = new Object(100000, fichero);
    obj->toGPU(program);
    scene->addObject(obj);
    camera->actualitzaCamera(scene->capsaMinima);
    updateGL();
}

// Finestra per a introduir una Light puntual o modificar-ne el seu contingut
void GLWidget::showAuxWindowPuntualLight(Light *light)
{
    auxWidget = new QWidget;
    auxWidget->move(0,0);

    QVBoxLayout *outer = new QVBoxLayout;
    QHBoxLayout *buttons = new QHBoxLayout;

    QGroupBox *groupBox = new QGroupBox("Light type");
    QRadioButton *radio1 = new QRadioButton("Position light");
    connect(radio1, SIGNAL(clicked()), this, SLOT(changePositionLight()));
    radio1->setChecked(true);

    QRadioButton *radio2 = new QRadioButton("Directional light");
    connect(radio2, SIGNAL(clicked()), this, SLOT(changeDirectionalLight()));
    radio2->setChecked(false); // Quan es clica es modifica el tipus de Light

    QRadioButton *radio3 = new QRadioButton("Spot light");
    connect(radio3, SIGNAL(clicked()), this, SLOT(changeSpotLight()));
    radio3->setChecked(false); // Sols quan es clica es modifica el tipus de  Light

    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(radio1);
    vbox->addWidget(radio2);
    vbox->addWidget(radio3);

    groupBox->setLayout(vbox);
    buttons->addWidget(groupBox);

    QGroupBox *groupBox2 = new QGroupBox("Selection");

    QRadioButton *transparent1 = new QRadioButton("&On");
    transparent1->setChecked(true);
    connect(transparent1, SIGNAL(clicked()), this, SLOT(activateLight()));

    QVBoxLayout *vbox2 = new QVBoxLayout;
    vbox2->addWidget(transparent1);

    groupBox2->setLayout(vbox2);
    buttons->addWidget(groupBox2);
    outer->addLayout(buttons);

    // light source intensity
    QSlider* lightSlider = new QSlider(Qt::Horizontal);
    lightSlider->setTickPosition(QSlider::TicksBelow);
    lightSlider->setMinimum(0);
    lightSlider->setMaximum(200);
    if (light == NULL)
        lightSlider->setSliderPosition(100);
    else
        lightSlider->setSliderPosition(100*light->getDiffuseIntensity()[0]);
    connect(lightSlider,SIGNAL(valueChanged(int)),this,SLOT(updateLightIntensity(int)));
    QLabel* lightLabel = new QLabel("Light intensity = ");
    QLabel* lightLabelValue = new QLabel();
    lightLabelValue->setNum(100 * light->getDiffuseIntensity()[0]);
    connect(lightSlider,SIGNAL(valueChanged(int)),lightLabelValue,SLOT(setNum(int)));
    QHBoxLayout *hboxLight = new QHBoxLayout;
    hboxLight->addWidget(lightLabel);
    hboxLight->addWidget(lightLabelValue);
    outer->addLayout(hboxLight);
    outer->addWidget(lightSlider);

    // X slider
    QSlider* XSlider = new QSlider(Qt::Horizontal);
    XSlider->setTickPosition(QSlider::TicksBelow);
    XSlider->setMinimum(-100);
    XSlider->setMaximum(100);
    if (light == NULL)
        XSlider->setSliderPosition(100);
    else
        XSlider->setSliderPosition(light->getLightPosition()[0]);
    connect(XSlider,SIGNAL(valueChanged(int)),this,SLOT(updateXPositionLight(int)));
    QLabel* XLabel = new QLabel("X Position = ");
    QLabel* XLabelValue = new QLabel();
    XLabelValue->setNum(0);
    connect(XSlider,SIGNAL(valueChanged(int)),XLabelValue,SLOT(setNum(int)));
    QHBoxLayout *hboxX = new QHBoxLayout;
    hboxX->addWidget(XLabel);
    hboxX->addWidget(XLabelValue);
    outer->addLayout(hboxX);
    outer->addWidget(XSlider);

    // Y slider
    QSlider* YSlider = new QSlider(Qt::Horizontal);
    YSlider->setTickPosition(QSlider::TicksBelow);
    YSlider->setMinimum(-100);
    YSlider->setMaximum(100);
    if (light == NULL)
        YSlider->setSliderPosition(100);
    else
        YSlider->setSliderPosition(light->getLightPosition()[1]);

    connect(YSlider,SIGNAL(valueChanged(int)),this,SLOT(updateYPositionLight(int)));
    QLabel* YLabel = new QLabel("Y Position = ");
    QLabel* YLabelValue = new QLabel();
    YLabelValue->setNum(0);
    connect(YSlider,SIGNAL(valueChanged(int)),YLabelValue,SLOT(setNum(int)));
    QHBoxLayout *hboxY = new QHBoxLayout;
    hboxY->addWidget(YLabel);
    hboxY->addWidget(YLabelValue);
    outer->addLayout(hboxY);
    outer->addWidget(YSlider);

    // Z Slider
    QSlider* ZSlider = new QSlider(Qt::Horizontal);
    ZSlider->setTickPosition(QSlider::TicksBelow);
    ZSlider->setMinimum(-100);
    ZSlider->setMaximum(100);
    if (light == NULL)
        ZSlider->setSliderPosition(100);
    else
        ZSlider->setSliderPosition(light->getLightPosition()[2]);
    connect(ZSlider,SIGNAL(valueChanged(int)),this,SLOT(updateZPositionLight(int)));
    QLabel* ZLabel = new QLabel("Z Position = ");
    QLabel* ZLabelValue = new QLabel();
    ZLabelValue->setNum(0);
    connect(ZSlider,SIGNAL(valueChanged(int)),ZLabelValue,SLOT(setNum(int)));
    QHBoxLayout *hboxZ = new QHBoxLayout;
    hboxZ->addWidget(ZLabel);
    hboxZ->addWidget(ZLabelValue);
    outer->addLayout(hboxZ);
    outer->addWidget(ZSlider);

    // Es poden afegir més camps a explorar amb el widget
    // Afegiu-los aqui sota

    auxWidget->setLayout(outer);
    auxWidget->show();
}

