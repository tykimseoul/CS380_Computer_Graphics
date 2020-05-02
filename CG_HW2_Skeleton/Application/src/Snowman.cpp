#include <Snowman.hpp>
#include <DefaultMaterial.hpp>
#include <PickableObject.hpp>
#include <BlenderObject.hpp>


Snowman::Snowman() {

}

Snowman::~Snowman() {

}

/*
 Construct snowman with head and body
 Parameters:
 h -> head
 b -> body
 */
void Snowman::ConstructSnowman(Sphere h, Sphere b) {
    auto *picking_material = new PickingMaterial();
    picking_material->CreateMaterial();

    h.SetPickingMat(picking_material);
    h.SetPosition(glm::vec3(0.0f, 2.2f, 0.0f));
    h.SetIndex(1);

    b.SetPickingMat(picking_material);
    b.SetPosition(glm::vec3(0.0f, -2.0f, 0.0f));
    b.SetIndex(2);
    h.AddParent(&b);

    head = &h;
    body = &b;
}

/*
 Add arms to the snowman
 Parameters:
 la -> left arm
 ra -> right arm
 */
void Snowman::AddArms(BlenderObject la, BlenderObject ra) {
    la.SetPosition(glm::vec3(1.3, 0.3, 0.0));
    la.SetOrientation(glm::rotate(glm::mat4(1.0), glm::radians(-60.0f), glm::vec3(0.0, 0.0, 1.0)));
    la.SetScale(glm::vec3(0.6));
    ra.SetPosition(glm::vec3(-1.3, 0.3, 0.0));
    ra.SetOrientation(glm::rotate(glm::mat4(1.0), glm::radians(60.0f), glm::vec3(0.0, 0.0, 1.0)));
    ra.SetScale(glm::vec3(0.6));

    la.SetIndex(3);
    ra.SetIndex(4);

    la.AddParent(body);
    ra.AddParent(body);

    leftArm = &la;
    rightArm = &ra;
}
/*
 Add face to the snowman
 Parameters:
 le -> left eye
 re -> right eye
 n -> nose
 m -> mouth
 */
void Snowman::AddFace(Sphere le, Sphere re, Cone n, Cylinder m) {
    le.SetPosition(glm::vec3(0.3, 0.2, 0.9));
    re.SetPosition(glm::vec3(-0.3, 0.2, 0.9));
    n.SetPosition(glm::vec3(0.0, 0.0, 1.0));
    n.SetOrientation(glm::rotate(glm::mat4(1.0), glm::radians(90.0f), glm::vec3(1.0, 0.0, 0.0)));
    m.SetPosition(glm::vec3(0.0, -0.4, 0.9));
    m.SetOrientation(glm::rotate(glm::mat4(1.0), glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0)));

    le.SetIndex(7);
    re.SetIndex(8);
    n.SetIndex(9);
    m.SetIndex(10);

    le.AddParent(head);
    re.AddParent(head);
    n.AddParent(head);
    m.AddParent(head);

    leftEye = &le;
    rightEye = &re;
    nose = &n;
    mouth = &m;
}

/*
 Add hat to the snowman
 Parameters:
 h -> hat BlenderObject
 */
void Snowman::AddHat(BlenderObject h) {
    h.SetPosition(glm::vec3(0.0, -4.2, 0.0));
    h.SetScale(glm::vec3(0.7));
    h.SetIndex(11);
    h.AddParent(head);
    hat = &h;
}

/*
 Add cane to the snowman
 Parameters:
 c -> cane BlenderObject
 an -> anchor for the cane rotation
 */
void Snowman::AddCane(BlenderObject c, Sphere an) {
    an.SetPosition(glm::vec3(-1.7, 0.5, 0.0));
    an.AddParent(body);
    caneAnchor = &an;

    c.SetPosition(glm::vec3(0.0, -0.35, 0.3));
    c.SetScale(glm::vec3(0.7));
    c.SetIndex(12);
    c.AddParent(caneAnchor);
    cane = &c;
}

/*
 Add bow tie to the snowman
 Parameters:
 tie -> bow tie BlenderObject
 */
void Snowman::AddBowTie(BlenderObject tie) {
    tie.SetPosition(glm::vec3(0.0, 1.4, 1.0));
    tie.SetOrientation(glm::rotate(glm::mat4(1.0), glm::radians(-30.0f), glm::vec3(1.0, 0.0, 0.0)));
    tie.SetScale(glm::vec3(0.4));
    tie.SetIndex(13);
    tie.AddParent(body);
    bowtie=&tie;
}

/*
 Render all RenderObjects of the snowman
 Parameters:
 camera -> camera to render for
 */
void Snowman::Render(Engine::Camera *camera) {
    head->Render(camera);
    body->Render(camera);
    leftArm->Render(camera);
    rightArm->Render(camera);
    leftEye->Render(camera);
    rightEye->Render(camera);
    nose->Render(camera);
    mouth->Render(camera);
    hat->Render(camera);
    cane->Render(camera);
    bowtie->Render(camera);
}

void Snowman::RenderPicking(Engine::Camera *camera) {
    head->RenderPicking(camera);
    body->RenderPicking(camera);
}

/*
 Wave left hand of the snowman between 40 and 70 degrees
 Parameters:
 deltaTime -> elapsed time in float
 */
void Snowman::WaveHand(float deltaTime) {
    if (handAngle <= 70 && handAngle >= 40) {
        handAngle += (deltaTime * 20.0f) * handDirection;
        leftArm->SetOrientation(glm::rotate(glm::mat4(1.0f), glm::radians(-handAngle), glm::vec3(0.0, 0.0, 1.0)));
    } else if (handAngle > 70) {
        handDirection = -handDirection;
        handAngle = 70.0f;
        leftArm->SetOrientation(glm::rotate(glm::mat4(1.0f), glm::radians(-handAngle), glm::vec3(0.0, 0.0, 1.0)));
    } else {
        handDirection = -handDirection;
        handAngle = 40.0f;
        leftArm->SetOrientation(glm::rotate(glm::mat4(1.0f), glm::radians(-handAngle), glm::vec3(0.0, 0.0, 1.0)));
    }
}

/*
 Swing cane of the snowman between -20 and 20 degrees
 Parameters:
 deltaTime -> elapsed time in float
 */
void Snowman::SwingCane(float deltaTime) {
    if (abs(caneAngle) <= 20) {
        caneAngle += (deltaTime * 20.0f) * caneDirection;
        caneAnchor->SetOrientation(glm::rotate(glm::mat4(1.0f), glm::radians(-caneAngle), glm::vec3(1.0, 0.0, 0.0)));
    } else if (caneAngle > 20) {
        caneDirection = -caneDirection;
        caneAngle = 20.0f;
        caneAnchor->SetOrientation(glm::rotate(glm::mat4(1.0f), glm::radians(-caneAngle), glm::vec3(1.0, 0.0, 0.0)));
    } else {
        caneDirection = -caneDirection;
        caneAngle = -20.0f;
        caneAnchor->SetOrientation(glm::rotate(glm::mat4(1.0f), glm::radians(-caneAngle), glm::vec3(1.0, 0.0, 0.0)));
    }
}




