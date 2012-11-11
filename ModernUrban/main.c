//
//  main.c
//  ModernUrban
//
//  Created by 何 靖恺 on 07/11/2012.
//  Copyright (c) 2012 何 靖恺. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <OpenGL/OpenGL.h>
#include <GLUT/GLUT.h>

#define PI 3.1415926

const int dir[4][2] = {1,0,-1,0,0,1,0,-1} ;
const GLfloat sceneSize = 500 ;
const int grid = 30 ;
int angleAlpha = 60 ;
int angleBeta = 30 ;
int angleBetaMax = 90 , angleBetaMin = -90 ;
GLfloat angleDistance = sceneSize * 0.65 ;
GLfloat angleDistanceMax = sceneSize , angleDistanceMin = 0 ;

int urbanMap[grid][grid] ;
int treecnt = 0 ;


void urbanMapInit(){
    for (int i = 0 ; i < grid ; ++ i) {
        for (int j = 0 ; j < grid ; ++ j) {
            if (i % 3 == 0 || i == grid - 1 || j == 0 || j == grid - 1) {
                urbanMap[i][j] = 0 ;
                //++treecnt ;
            }
            else if (i % 3 == 1 ){
                int rate = rand() % 100 ;
                if (rate <= 80) {
                    urbanMap[i][j] = 1 ;
                }else{
                    urbanMap[i][j] = 0 ;
                    //++treecnt ;
                }
                
            }else{
                urbanMap[i][j] = urbanMap[i-1][j] ;
                //if (urbanMap[i][j] == 0) ++treecnt ;
            }
        }
    }
}

void urbanDebug(){
    for (int i = 0; i < grid; ++ i) {
        for (int j = 0 ; j < grid; ++ j) {
            printf("%d" , urbanMap[i][j]) ;
        }
        printf("\n") ;
    }
}


void drawCube(GLfloat size){
    GLfloat vertex[8][4] = {
        { -0.5 , -0.5 , 0.5 , 1 } ,
        { 0.5 , -0.5 , 0.5 , 1 } ,
        { 0.5 , -0.5 , -0.5 , 1 } ,
        { -0.5 , -0.5 , -0.5 , 1 } ,
        { -0.5 , 0.5 , 0.5 , 1 } ,
        { 0.5 , 0.5 , 0.5 , 1 } ,
        { 0.5 , 0.5 , -0.5 , 1 } ,
        { -0.5 , 0.5 , -0.5 , 1 }
    } ;
    
    GLfloat normaldir[6][4] = {
        {0 , -1 , 0 , 1 } ,
        {0 , 1 , 0 , 1 } ,
        {0 , 0 , 1 , 1 } ,
        {1 , 0 , 0 , 1 } ,
        {0 , 0 , -1 , 1 } ,
        {-1 , 0 , 0 , 1 }
    };
    
    int pnts[6][4] = {
        { 0 , 1 , 2 , 3 } ,
        { 4 , 5 , 6 , 7 } ,
        { 0 , 1 , 5 , 4 } ,
        { 1 , 2 , 6 , 5 } ,
        { 3 , 2 , 6 , 7 } ,
        { 0 , 3 , 7 , 4 }
    } ;
    
    for (int i = 0 ; i < 8; ++ i) {
        for (int j = 0 ; j < 3; ++j) {
            vertex[i][j] *= size ;
        }
    }
    
    for (int i = 0; i < 6; ++ i) {

        glBegin(GL_POLYGON) ;
        glNormal3fv(normaldir[i]) ;
        for ( int j = 0 ; j < 4 ; ++ j){
            //glNormal3fv(normal) ;
            glVertex3fv(vertex[pnts[i][j]]) ;
        }
        glEnd() ;
    }
}
void init(void){
    
    GLfloat ambient[] = {0.0 , 0.0 , 0.0 , 1.0 } ;
    GLfloat white_light[] = {1.0 , 1.0 , 1.0 , 1.0 } ;
    GLfloat diffuse[] = {0.4 , 0.4 , 0.4 ,1.0 } ;
    GLfloat lmodel_ambient[] = {0.4 ,0.4 , 0.4 , 1.0 } ;
    GLfloat local_view[] = {0.0} ;
 
    GLfloat light_position0 [] = { sceneSize / 2 , sceneSize * 0.25 , sceneSize / 2, 1.0 } ;

    glClearColor(0.77, 0.90, 0.94, 0.2) ;
    glEnable(GL_DEPTH_TEST) ;
    glShadeModel(GL_SMOOTH);
    
    glColorMaterial(GL_FRONT, GL_DIFFUSE) ;
    
    glLightfv(GL_LIGHT0, GL_POSITION, light_position0) ;
    glLightfv(GL_LIGHT0, GL_COLOR, white_light) ;
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse) ;
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient) ;
    glEnable(GL_LIGHT0) ;
 
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient) ;
    glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, local_view) ;
    
    glEnable(GL_LIGHTING );
    
    
    /* 
     * Code for anti aliasing, I found the code from the Internet,
     * however can't figure out what the site called now.
     */
    
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST); // Make round points, not square points
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);  // Antialias the lines
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void drawCycle(GLfloat radius , int slices){
    
    GLfloat deltaAngle = PI * 2 / slices ;
    glBegin(GL_TRIANGLE_FAN) ;
    glNormal3f(0.0, 1.0, 0.0) ;
    glVertex4f(0.0, 0.0, 0.0 , 1.0) ;
    for ( int i = 0 ; i < slices ; ++ i){
        glVertex4f(0.0 + radius * cos(deltaAngle * i), 0.0 + radius * sin(deltaAngle * i) , 0.0 , 1.0 ) ;
    }
    glVertex4f(radius, 0.0, 0.0, 1.0) ;
    glEnd() ;
    
}

void drawCylinder(GLfloat base , GLfloat top , GLfloat height) {
    glPushMatrix() ;
    
    glRotated(-90, 1.0, 0.0, 0.0) ;
    gluCylinder(gluNewQuadric(), base, top, height, 20, 16 ) ;
    glPushMatrix() ;
    //glTranslatef(0.0, 0.0, height) ;
    drawCycle(base, 20) ;
    glPopMatrix() ;
    
    glPushMatrix() ;
    glTranslatef(0.0, 0.0, height) ;
    drawCycle(top, 20) ;
    glPopMatrix() ;
    
    glPopMatrix() ;
}

void drawBranch(GLfloat length){
    drawCylinder(length / 40, length / 160, length) ;
}

const int treeNumber = grid * grid * 4 ;

int treeRotatedAngle[treeNumber][30] ;
int branchRotatedAngle[treeNumber][30] ;

void treesInit(){
    for (int i = 0 ; i < treeNumber; ++ i) {
        for ( int j = 0 ; j < 30 ; ++ j){
            treeRotatedAngle[i][j] = rand() % 45 ;
        }
    }
    for (int i = 0 ; i < treeNumber; ++ i) {
        for (int j = 0; j < 30; ++ j) {
            branchRotatedAngle[i][j] = rand() % 60 ;
        }
    }
}

void drawTree(GLfloat length , int depth , int randid){
    glPushMatrix() ;
    drawBranch(length) ;
    if ( depth != 0) {
        glTranslatef(0.0, length / 3, 0.0) ;
        glPushMatrix() ;
        glRotated(branchRotatedAngle[randid][depth + 1], 0.0, 1.0, 0.0) ;
        glRotated(-treeRotatedAngle[randid][depth], 0.0, 0.0, 1.0) ;
        drawTree(length / 2, depth - 1 , randid) ;
        glPopMatrix() ;
        
        glPushMatrix() ;
        glRotated(branchRotatedAngle[randid][depth + 2], 0.0, 1.0, 0.0) ;
        glRotated(treeRotatedAngle[randid][depth], 0.0, 0.0, 1.0) ;
        drawTree(length / 2, depth - 1 , randid) ;
        
        
        glPopMatrix() ;
    }
    glPopMatrix() ;
}

const int buildingNumber = grid * grid ;
int buildingRandNumber[buildingNumber] ;
int buildingRandAngle[buildingNumber] ;
GLfloat buildingRandHeight[buildingNumber] ;
GLfloat buildingRandDepth[buildingNumber] ;
void buildingInit(){
    
    for(int i = 0 ; i < buildingNumber ; ++ i){
        buildingRandNumber[i] = rand() % 110 ;
        buildingRandAngle[i] = rand() % 4 + 1 ;
        buildingRandDepth[i] = 1 + rand() % 3 ;
    }
    for (int i = 0; i < buildingNumber; ++ i) {
        int tmp = rand() % 2 == 1 ? 1 : -1 ;
        buildingRandHeight[i] = 1 + (GLfloat)(rand() % 100) / 100 * 0.20  * tmp;
        //printf("%lf\n" , buildingRandHeight[i]) ;
    }
}


//building style 1st

void drawCubeBuildingMainPart(GLfloat base_a , GLfloat base_b , GLfloat height , int level){
    glPushMatrix() ;
    
    glPushMatrix() ;
    glTranslatef(0.0, height / 2, 0.0) ;
    glScalef(base_a, height, base_b) ;
    //drawUnitCube() ;
    drawCube(1.0) ;
    //glPushMatrix() ;
    glPopMatrix() ;
    
    if ( level != 0){
        glPushMatrix() ;
        glTranslatef(0.0, height * 0.1 , 0.0) ;
        drawCubeBuildingMainPart(base_a* 0.7, base_b * 0.7, height, level-1 ) ;
        glPopMatrix() ;
    }
    
    glPopMatrix() ;
}

//building style 2nd

void drawCubeBuildingHotel(GLfloat base_a , GLfloat base_b , GLfloat height , int level){
    glPushMatrix() ;
    
    glPushMatrix() ;
    glTranslatef(0.0, height / 2, 0.0) ;
    glScaled(base_a, height, base_b) ;
    //drawUnitCube() ;
    drawCube(1.0) ;
    glPopMatrix() ;
    
    if(level!= 0){
        glPushMatrix() ;
        glTranslatef(base_a / 3, 0.0, 0.0) ;
        drawCubeBuildingHotel(0.8 * base_b, 0.8 * base_b,  height* 0.9, level - 1) ;
        glPopMatrix() ;
    }
    glPopMatrix() ;
}

//building style 3rd

void drawMultiBuilding(GLfloat base_a , GLfloat base_b , GLfloat height , int level){
    glPushMatrix() ;
    
    glPushMatrix() ;
    glTranslatef(0.0, height / 2, 0.0) ;
    glScalef(base_a, height, base_b) ;
    //drawUnitCube() ;
    drawCube(1.0) ;
    glPopMatrix() ;
    
    glPushMatrix() ;
    glTranslatef(base_a / 2, height * 0.4,  base_b / 2) ;
    glScalef(base_a / 4, height * 0.8 , base_b / 4) ;
    //drawUnitCube() ;
    drawCube(1.0) ;
    glPopMatrix() ;

    glPushMatrix() ;
    glTranslatef(-base_a / 2, height * 0.4, - base_b / 2) ;
    glScalef(base_a / 4, height * 0.8 , base_b / 4) ;
    //drawUnitCube() ;
    drawCube(1.0) ;
    glPopMatrix() ;
    
    if ( level != 0){
        glPushMatrix() ;
        glTranslatef(base_a / 2, 0.0, - base_b / 2) ;
        drawMultiBuilding(base_a / 4, base_b / 4, height * 0.8, level - 1) ;
        glPopMatrix() ;
    }
    glPopMatrix() ;
}

//building style 4th

void drawDepartment(GLfloat base , GLfloat height , int level){
    glPushMatrix() ;
    
    drawCylinder(base, base, height);
    glPushMatrix() ;
    glTranslatef(0.0, height * 3 / 4, 0.0) ;
    for (int i = 0; i < 2; ++ i) {
        glPushMatrix() ;
        glRotatef(90 * i, 0.0, 1.0, 0.0) ;
        glScalef(base * 0.7, base * 0.7 , base * 8) ;
        //drawUnitCube() ;
        drawCube(1.0) ;
        glPopMatrix() ;
    }
    glPopMatrix() ;
    if (level != 0) {
        glPushMatrix() ;
        glTranslatef(0.0, height / 2, 0.0) ;
        for (int i = 0 ; i < 4; ++ i) {
            glPushMatrix() ;
            glRotatef(90 * i, 0.0, 1.0, 0.0) ;
            glTranslatef(base * 4, 0.0, 0.0) ;
            //drawCylinder(base/2, base/2, height/2) ;
            drawDepartment(base / 2, height / 2, level - 1) ;
            glPopMatrix() ;
        }
        glPopMatrix() ;
    }
    
    glPopMatrix() ;
}

//building style 5th
void drawEmpireStateBuilding(GLfloat base_a , GLfloat base_b , GLfloat height){
    
    glPushMatrix() ;
    drawCubeBuildingMainPart(base_a, base_b, height, 3) ;
    
    int tmp[4][2] = {1,1,1,-1,-1,-1,-1,1} ;
    for (int i = 0 ; i < 4; ++ i) {
        glPushMatrix();
        glTranslatef(base_a / 2 * tmp[i][0], 0.0, base_b / 2 * tmp[i][1]) ;
        glRotatef(90 * (i+3), 0.0, 1.0, 0.0);
        drawMultiBuilding(base_a / 4, base_b / 4, height * 0.8, 2) ;
        glPopMatrix();
    }
    
    glPopMatrix() ;
}

//building style 6th

void drawTemple(GLfloat base_a , GLfloat base_b , GLfloat height , GLfloat level ){
    glPushMatrix() ;
    glTranslatef(0.0, height / 2, 0.0) ;
    glPushMatrix() ;
    glScaled(base_a, height, base_b) ;
    drawCube(1.0) ;
    glPopMatrix() ;
    

    for (int i = 0 ; i < level ; ++ i) {
        glPushMatrix() ;
        //glRotatef(45 * i, 0.0, 1.0, 0.0) ;
        glTranslatef(0.0, height / 2 + i * height / 6, 0.0) ;
        glScalef(base_a * (2 + (GLfloat)i /level), height / 3, base_b * (2 + (GLfloat)i / level) ) ;
        drawCube(1.0) ;
        glPopMatrix() ;
    }
    glPopMatrix() ;
}


GLfloat no_mat[] = { 0.0 , 0.0 , 0.0 , 1.0 } ;
GLfloat mat_ambient [] = { 0.7 , 0.7 , 0.7 , 1.0 } ;
GLfloat mat_ambient_color [] = { 0.3 , 0.3 , 0.3 , 1.0 } ;
GLfloat mat_diffuse [] = { 0.1 , 0.5 , 0.8 , 1.0 } ;
GLfloat mat_specular [] = { 1.0 , 1.0 , 1.0 , 1.0 } ;
GLfloat no_shininess [] = { 0.0 } ;
GLfloat low_shininess [] = {5.0 } ;
GLfloat high_shininess [] = { 100.0 } ;
GLfloat mat_emission [] = { 0.3 , 0.2 , 0.2 , 0.0 } ;

void generate(GLfloat *ambient , GLfloat *specular ,
              GLfloat *shininess , GLfloat * emission){
    
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient) ;
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse) ;
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular) ;
    glMaterialfv(GL_FRONT, GL_SHININESS, shininess) ;
    glMaterialfv(GL_FRONT, GL_EMISSION, emission) ;
}

GLfloat greenTree[4] = { 0.45 , 0.25 , 0.0 , 1.0 } ;
GLfloat brickRoad[4] = { 0.0 , 0.0 , 0.0 , 1.0 } ;
GLfloat greenGround[4] = { 0.0 , 1.0 , 0.0 , 1.0 } ;
GLfloat building[4] = { 0.7 , 0.7 , 0.7 , 0.3 } ;

void placeTree(int x , int y , GLfloat base_a , GLfloat base_b , GLfloat thick ,int randid){
    GLfloat area_a = base_a / grid ;
    GLfloat area_b = base_b / grid ;
    int treeStaticIter = 0 ;
    
    /*
     glPushMatrix() ;
     glTranslatef(area_a * x + area_a / 2 - base_a / 2 ,
     thick * 0.55 ,
     area_b * y + area_b / 2 - base_b / 2 ) ;
     drawTree(sqrtf(base_a * base_b) / grid, 3, randid) ;
     glPopMatrix() ;
     */
    
    for (int i = 0; i < 4; ++i) {
        int tmpx = x + dir[i][0] ;
        int tmpy = y + dir[i][1] ;
        //printf("%d %d\n" , tmpx , tmpy) ;
        if( tmpx >= 0 && tmpx < grid && tmpy >= 0 && tmpy < grid){
            if(urbanMap[tmpx][tmpy] == 1){
                glPushMatrix() ;
                glTranslatef(area_a * x + area_a / 2 - base_a / 2 + area_a * 0.6 / 2 * dir[i][0],
                             thick * 0.55 ,
                             area_b * y + area_b / 2 - base_b / 2 + area_b * 0.6 / 2 * dir[i][1]) ;
                generate(greenTree, mat_ambient_color, no_shininess, mat_emission) ;
                drawTree(sqrtf(base_a * base_b) / (2 *grid) , 3, treeStaticIter ++) ;
                
                glPushMatrix() ;
                generate(greenGround, mat_specular, low_shininess , mat_emission) ;
                glTranslatef(0.0, -0.5 * thick, 0.0) ;
                if (dir[i][0] != 0){
                    glRotated(90, 0.0, 1.0, 0.0) ;
                }
                glScalef(sqrtf(area_a * area_b) * 1.54, thick * 1.2, sqrtf(base_a * base_b) / (grid * grid) * 4) ;
                drawCube(1.0) ;
                glPopMatrix() ;
                
                glPopMatrix() ;
                
            }
        }
    }
}

GLuint treelists;

void displayListInit(){
    treecnt = 0 ;
    for (int i = 0 ; i < grid; ++i) {
        for (int j = 0 ; j < grid; ++j) {
            if( urbanMap[i][j] == 0){
                ++treecnt ;
            }
        }
    }
    treelists = glGenLists(treecnt) ;
    glListBase(treelists) ;
    int tmpIter = 0 ;
    for (int i = 0 ; i < grid; ++ i) {
        for (int j = 0; j < grid; ++ j) {
            if (urbanMap[i][j] == 0) {
                glNewList(treelists + tmpIter, GL_COMPILE) ;
                placeTree(i, j, sceneSize / 2, sceneSize / 2, 1.0 , 1) ;
                glEndList() ;
            }
        }
    }
}

void drawBuilding(int area_a , int area_b , int height ,int style, int randAngle , int randHeightRate , int level ){
    
    GLfloat buildingColor[6][4] = {
        { 0.5 , 0.5 , 0.5 , 1 } ,
        { 0.4 , 0.4 , 0.4 , 1 } ,
        { 0.3 , 0.3 , 0.3 , 1 } ,
        { 0.2 , 0.2 , 0.2 , 1 } ,
        { 0.7 , 0.7 , 0.7 , 1 } ,
        { 0.6 , 0.6 , 0.6 , 1 }
    } ;
    if(style >= 0 && style < 30){
        
        glPushMatrix() ;
        generate(buildingColor[0], mat_specular, low_shininess, no_mat) ;
        drawCubeBuildingMainPart(area_a, area_b, height * 2, level) ;
        glPopMatrix() ;
    }
    
    else if (style >= 30 && style < 60){
        
        glPushMatrix() ;
        generate(buildingColor[1], mat_specular, high_shininess, no_mat) ;
        glRotatef(90 * randAngle, 0.0, 1.0, 0.0) ;
        drawCubeBuildingHotel(area_a / 3, area_b / 3, height, level) ;
        glPopMatrix() ;
    }
    
    else if (style >= 60 && style < 75){
        
        glPushMatrix() ;
        generate(buildingColor[2], mat_specular, high_shininess, no_mat) ;
        glRotatef(90 * randAngle, 0.0, 1.0, 0.0) ;
        drawMultiBuilding(area_a / 2, area_b / 2, height * 2, level ) ;
        glPopMatrix() ;
    }
    
    else if (style >= 75 && style < 78 ){
        
        glPushMatrix() ;
        generate(buildingColor[3], mat_specular, high_shininess, no_mat) ;
        drawDepartment(area_a * area_b / 30, height * 3 * randHeightRate, level) ;
        glPopMatrix() ;
    }
    
    else if (style >= 78 && style < 100){
        
        glPushMatrix() ;
        generate(buildingColor[4], mat_specular, high_shininess, no_mat) ;
        drawEmpireStateBuilding(area_a / 1.5, area_b / 1.5, height * level) ;
        glPopMatrix() ;
    }
    else if (style >= 100 && style < 110){
        glPushMatrix() ;
        generate(buildingColor[5], mat_specular, high_shininess, no_mat) ;
        drawTemple(area_a / 2, area_b / 2, height / 4, level  * 10) ;
        glPopMatrix() ;
    }
}

void drawMapBase(GLfloat base_a , GLfloat base_b , GLfloat thick){
    glPushMatrix() ;
    generate(brickRoad, mat_specular, low_shininess , mat_emission) ;
    glScalef(base_a, thick, base_b) ;
    //drawUnitCube() ;
    drawCube(1.0) ;
    glPopMatrix() ;
}


void drawRoadOnMap(GLfloat base_a , GLfloat base_b , GLfloat thick){
    int buildingStaticInter = 0 ;
    int treeStaticIter = 0 ;
    GLfloat area_a = base_a / grid ;
    GLfloat area_b = base_b / grid ;
    
    //printf("%f %f\n" ,area_a , area_b) ;
    glPushMatrix() ;
    drawMapBase(base_a, base_b, thick) ;
    for (int i = 0; i < grid; ++ i) {
        for (int j = 0 ; j < grid; ++ j) {
            if (urbanMap[i][j] == 1) {
                glPushMatrix() ;
                //printf("%f %f %f\n" ,area_a * ( i - 1 ) + area_a / 2 - base_a / 2 , \
                       thick * 0.2 , \
                       area_b * ( j - 1 ) + area_b / 2 - base_b / 2) ;
                glTranslatef(area_a * i + area_a / 2 - base_a / 2 ,
                             thick * 0.05 ,
                             area_b * j + area_b / 2 - base_b / 2) ;
                
                glPushMatrix() ;
                //glTranslatef(0.0, thick * 5, 0.0) ;
                //generate(no_mat, mat_specular, low_shininess, mat_emission) ;
                drawBuilding(area_a * 0.9, area_b * 0.9,
                             sqrtf(area_a * area_b),
                             buildingRandNumber[buildingStaticInter],
                             buildingRandNumber[buildingStaticInter],
                             buildingRandHeight[buildingStaticInter],
                             buildingRandDepth[buildingStaticInter++]);
                
                glPopMatrix() ;
            
                generate(brickRoad, mat_specular, low_shininess , mat_emission) ;
                glScalef(area_a, thick * 1.2, area_b) ;
                //drawUnitCube() ;
                drawCube(1.0) ;
                glPopMatrix() ;
            }else if(urbanMap[i][j] == 0){

                placeTree(i, j, base_a, base_b, thick, 1) ;
                treeStaticIter ++ ;
            }
        }
    }
    glPopMatrix() ;
}

void drawSkyLine(GLfloat size){
    GLfloat vertex[8][4] = {
        {-size / 2, 0 , size / 2 , 1 },
        {size / 2 , 0 , size / 2 , 1},
        {size / 2 , 0 , - size / 2 , 1},
        {-size / 2 , 0 , - size / 2 , 1},
        {-size / 2 , size / 4, size / 2 , 1},
        {size / 2 , size / 4, size / 2 , 1},
        {size / 2 , size / 4, - size / 2 , 1},
        {-size / 2 , size / 4, - size / 2 , 1}
    };
    glPushMatrix() ;
    generate(no_mat, mat_specular, low_shininess, mat_emission) ;
    glBegin(GL_LINE_LOOP) ;
    glVertex3fv(vertex[0]) ;
    glVertex3fv(vertex[1]) ;
    glVertex3fv(vertex[5]) ;
    glVertex3fv(vertex[4]) ;
    glEnd();
    
    glBegin(GL_LINE_LOOP) ;
    glVertex3fv(vertex[1]) ;
    glVertex3fv(vertex[2]) ;
    glVertex3fv(vertex[6]) ;
    glVertex3fv(vertex[5]) ;
    glEnd();

    glBegin(GL_LINE_LOOP) ;
    glVertex3fv(vertex[3]) ;
    glVertex3fv(vertex[2]) ;
    glVertex3fv(vertex[6]) ;
    glVertex3fv(vertex[7]) ;
    glEnd();

    glBegin(GL_LINE_LOOP) ;
    glVertex3fv(vertex[0]) ;
    glVertex3fv(vertex[3]) ;
    glVertex3fv(vertex[7]) ;
    glVertex3fv(vertex[4]) ;
    glEnd();

    glBegin(GL_LINE_LOOP) ;
    glVertex3fv(vertex[4]) ;
    glVertex3fv(vertex[5]) ;
    glVertex3fv(vertex[6]) ;
    glVertex3fv(vertex[7]) ;
    glEnd();
    glPopMatrix() ;

}

void display(void){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) ;
    glPushMatrix() ;
    
     gluLookAt(angleDistance * cos(PI * angleBeta / 180 ) * cos(PI * angleAlpha / 180 ),
              angleDistance * sin(PI * angleBeta / 180),
              angleDistance * cos(PI * angleBeta / 180 ) * sin(PI * angleAlpha / 180 ),
              0.0, 0.0, 0.0,
              0.0, 1.0, 0.0) ;
    
    glTranslatef(0.0, -5.0, 0.0) ;
    
    drawSkyLine(sceneSize / 2) ;
    glPushMatrix() ;
    //drawCubeBuildingMainPart(5, 5, 10, 3);
    //drawCubeBuildingHotel(5, 5, 10, 4);
    //generate(greenTree, mat_ambient_color, no_shininess, mat_emission) ;
    //drawTree(20, 5, 1);
    //drawMultiBuilding(30, 30, 60, 3) ;
    //drawEmpireStateBuilding(30, 30, 90);
    //drawDepartment(2, 20, 3);
    //drawMapBase(30 , 30 , 1.0) ;
    //drawTemple(10, 10, 10, 10) ;
    drawRoadOnMap(sceneSize / 2, sceneSize / 2, 1.0) ;
    glPopMatrix() ;
    
    glPopMatrix();
    glFlush() ;
}

void reshape(int w , int h){
    glViewport(0.0, 0.0, (GLsizei)w , (GLsizei)h) ;
    glMatrixMode(GL_PROJECTION) ;
    glLoadIdentity() ;
    gluPerspective(60, (GLfloat)w / (GLfloat)h ,1.0, sceneSize) ;
    glMatrixMode(GL_MODELVIEW) ;
    glLoadIdentity() ;
}

void keyboard(unsigned char key , int x , int y) {
    switch (key) {
        case 27:
            exit(0) ;
            break;
         
        case '1':
            angleAlpha = (angleAlpha + 3) % 360 ;
            glutPostRedisplay() ;
            break ;
            
        case '2':
            angleAlpha = ( angleAlpha - 3 + 360) % 360 ;
            glutPostRedisplay() ;
            break ;
            
        case '3':
            if ( angleBeta + 1 <= angleBetaMax){
                angleBeta += 1 ;
                glutPostRedisplay() ;
            }
            break ;
            
        case '4':
            if ( angleBeta - 1 >= angleBetaMin){
                angleBeta -= 1 ;
                glutPostRedisplay() ;
            }
            break ;
            
        case '5':
            if ( angleDistance + 5 <= angleDistanceMax){
                angleDistance += 5 ;
                glutPostRedisplay() ;
            }
            break ;
            
        case '6' :
            if ( angleDistance - 5 >= angleDistanceMin){
                angleDistance -= 5 ;
                glutPostRedisplay() ;
            }
            break ;
                        
        default:
            break;
    }
}

//print the instruments

void instruments(){
    puts(" 1  ---> Change the view horizontally to right") ;
    puts(" 2  ---> Change the view horizontally to left") ;
    puts(" 3  ---> Change the view vertically to up side") ;
    puts(" 4  ---> Change the view vertically to down side") ;
    puts(" 5  ---> Lengthen the distance to the city centre") ;
    puts(" 6  ---> Reduce the distance to the city centre") ;
    puts("esc ---> Exit") ;
}

int main(int argc, char * argv[])
{
    instruments() ;
    srand((unsigned)time(NULL)) ;
    treesInit() ;
    urbanMapInit() ;
    buildingInit() ;
    //urbanDebug() ;
    glutInit(&argc, argv) ;
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH) ;
    glutInitWindowPosition(100, 100) ;
    glutInitWindowSize(800, 600) ;
    glutCreateWindow("This is the URBAN") ;
    init() ;
    displayListInit() ;
    glutDisplayFunc(display) ;
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard) ;
    glutMainLoop() ;
    return 0;
}
