#ifndef _RUBIX_CUBE_H
#define _RUBIX_CUBE_H

#include <array>
#include "model.h"
#include "Shader.h"
#include "modelLoading.h"

enum modelNames{
    RCC = 1, LCC, UCC, RTC, LTC, LCB, DCC, RBC,
    BTL, BRB, BCL, BLB, BCR, BTC, BCB, BCC, BTR,
    FCR, FCL, FTC, FBC, FTL, FBL, FBR, FCC, FTR
};
/// Names for all of the rotations 
enum Moves
{
      L, LPRIME, L2,    // left, reverse left, twice left
      R, RPRIME, R2,    // right  //           // 
      U, UPRIME, U2,    // up,    //           //
      D, DPRIME, D2,    // down,  //           //
      F, FPRIME, F2,    // front, //           //
      B, BPRIME, B2,    // Back,  //           //
      Y, YPRIME, Y2,    // Yaxis  //           //
      X, XPRIME, X2,    // Xaxis  //           //
      Z, ZPRIME, Z2,    // Zaxis  //           //
      M, MPRIME, M2,    // middleY //           //
      E, EPRIME, E2,    // middleX //           //
      S, SPRIME, S2,     // other middle //           //
      NO                 // No move
};

glm::vec3 axisOfRotations[] = {
    glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(-1.0f,  0.0f,  0.0f),
    glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3( 1.0f,  0.0f,  0.0f),
    glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3( 0.0f,  1.0f,  0.0f),
    glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3( 0.0f, -1.0f,  0.0f),
    glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3( 0.0f,  0.0f,  1.0f),
    glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3( 0.0f,  0.0f, -1.0f),
    glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3( 0.0f,  1.0f,  0.0f),
    glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3( 1.0f,  0.0f,  0.0f),
    glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3( 0.0f,  0.0f,  1.0f),
    glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(-1.0f,  0.0f,  0.0f),
    glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3( 0.0f, -1.0f,  0.0f),
    glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3( 0.0f,  0.0f,  1.0f),
};

unsigned int steps = 90 ;

class RubixCube
{
private:
    vector<Model> models;
public:
    RubixCube()
    {
        this->models =  ModelLoading().GetModels();
    }

    void Draw(Shader shader, glm::mat4 view, Moves move = NO)
    {
        /// by far every thing is fine except cross rotation of two moves where 
        /// it losses its orientation. Quaternions need a through understanding
            // not rotating 
            pair<pair<vector<int>, vector<int>>, glm::vec3> p = Movement(move);
            for(int i: p.first.first)
            {
                glm::mat4 rotView = view * this->models[i].quaterinion.rotMat4;
                shader.setMat4("view", rotView);
                this->models[i].Draw(shader);
            }
            for(int i: p.first.second)
            {
                glm::mat4 rotView = view * this->models[i].quaterinion.QuatRotMatrix4f(glm::vec4(p.second, steps));
                shader.setMat4("view", rotView);
                this->models[i].Draw(shader);
            } 
            if (move != NO)
                update(move);
       
    }

    void update(Moves move)
    {
        if (move == L)
        {
            LUpdate();
        }
        else if (move == R)
        {
            RUpdate();
        }
        else if (move == U)
        {
            UUpdate();
        }
        else if (move == D)
        {
            DUpdate();
        }
        else if (move == F)
        {
            FUpdate();
        }
        else if (move == B)
        {
            BUpdate();
        }
    }

    pair<pair<vector<int>, vector<int>>, glm::vec3> Movement(Moves move)
    {
        if (move == L)
        {
           return make_pair(LMove(), axisOfRotations[move]);
        }
        else if (move == R)
        {
            return make_pair(RMove(), axisOfRotations[move]);
        }else if (move == U)
        {
            return make_pair(UMove(), axisOfRotations[move]);
        }else if (move == D)
        {
            return make_pair(DMove(), axisOfRotations[move]);
        }else if (move == F)
        {
            return make_pair(FMove(), axisOfRotations[move]);
        }else if (move == B)
        {
            return make_pair(BMove(), axisOfRotations[move]);
        }
        else
         return make_pair(NoMove(), glm::vec3(0.0));
    }

    void LUpdate()
    {
        Model hold        = this->models[FTL];
        this->models[FTL] = this->models[BTL];
        this->models[BTL] = this->models[BLB];
        this->models[BLB] = this->models[FBL];
        this->models[FBL] = hold;

        hold              = this->models[FCL];
        this->models[FCL] = this->models[LTC];
        this->models[LTC] = this->models[BCL];
        this->models[BCL] = this->models[LCB];
        this->models[LCB] = hold;
    }
    void RUpdate()
    {
        Model hold        = this->models[FTR];
        this->models[FTR] = this->models[FBR];
        this->models[FBR] = this->models[BRB];
        this->models[BRB] = this->models[BTR];
        this->models[BTR] = hold;

        hold              = this->models[FCR];
        this->models[FCR] = this->models[RBC];
        this->models[RBC] = this->models[BCR];
        this->models[BCR] = this->models[RTC];
        this->models[RTC] = hold;
    }
    void UUpdate()
    {
        Model hold        = this->models[FTR];
        this->models[FTR] = this->models[BTR];
        this->models[BTR] = this->models[BTL];
        this->models[BTL] = this->models[FTL];
        this->models[FTL] = hold;

        hold              = this->models[FTC];
        this->models[FTC] = this->models[RTC];
        this->models[RTC] = this->models[BTC];
        this->models[BTC] = this->models[LTC];
        this->models[LTC] = hold;
    }
    void DUpdate()
    {
        Model hold        = this->models[FBR];
        this->models[FBR] = this->models[FBL];
        this->models[FBL] = this->models[BLB];
        this->models[BLB] = this->models[BRB];
        this->models[BRB] = hold;

        hold              = this->models[FBC];
        this->models[FBC] = this->models[LCB];
        this->models[LCB] = this->models[BCB];
        this->models[BCB] = this->models[RBC];
        this->models[RBC] = hold;
    }
    void FUpdate()
    {
        Model hold        = this->models[FBR];
        this->models[FBR] = this->models[FTR];
        this->models[FTR] = this->models[FTL];
        this->models[FTL] = this->models[FBL];
        this->models[FBL] = hold;

        hold              = this->models[FBC];
        this->models[FBC] = this->models[FCR];
        this->models[FCR] = this->models[FTC];
        this->models[FTC] = this->models[FCL];
        this->models[FCL] = hold;
    }
    void BUpdate()
    {
        Model hold        = this->models[BTR];
        this->models[BTR] = this->models[BRB];
        this->models[BRB] = this->models[BLB];
        this->models[BLB] = this->models[BTL];
        this->models[BTL] = hold;

        hold              = this->models[BTC];
        this->models[BTC] = this->models[BCR];
        this->models[BCR] = this->models[BCB];
        this->models[BCB] = this->models[BCL];
        this->models[BCL] = hold;
    }
   

    pair<vector<int>, vector<int>> LMove()
    {
        vector<int> rotating    = {FTL, LTC, BTL, BCL, BLB, LCB, FBL, FCL, LCC};
        vector<int> nonRotating = {RCC, UCC, RTC, DCC, RBC, BRB, BCR, BTC, BCB, BCC, BTR, FCR, FTC, FBC, FBR, FCC, FTR};
        return make_pair(nonRotating, rotating);
    }
    pair<vector<int>, vector<int>> RMove()
    {
        vector<int> rotating    =  {FTR, RTC, BTR, BCR, BRB, RBC, FBR, FCR, RCC};
        vector<int> nonRotating =  {LCC, UCC, LTC, LCB, DCC, BTL, BCL, BLB, BTC, BCB, BCC, FCL, FTC, FBC, FTL, FBL, FCC};
        return make_pair(nonRotating, rotating);
    }
    pair<vector<int>, vector<int>> UMove()
    {
        vector<int> rotating    =  {FTL, LTC, BTL, BTC, BTR, RTC, FTR, FTC, UCC};
        vector<int> nonRotating =  {RCC, LCC, LCB, DCC, RBC, BRB, BCL, BLB, BCR, BCB, BCC, FCR, FCL, FBC, FBL, FBR, FCC};
        return make_pair(nonRotating, rotating);
    }
    pair<vector<int>, vector<int>> DMove()
    {
        vector<int> rotating    =  {FBL, LCB, BLB, BCB, BRB, RBC, FBR, FBC, DCC};
        vector<int> nonRotating =  {RCC, LCC, UCC, RTC, LTC, BTL, BCL, BCR, BTC, BCC, BTR, FCR, FCL, FTC, FTL, FCC, FTR};
        return make_pair(nonRotating, rotating);
    }
    pair<vector<int>, vector<int>> FMove()
    {
        vector<int> rotating    =  {FTR, FTC, FTL, FCL, FBL, FBC, FBR, FCR, FCC};
        vector<int> nonRotating =  {RCC, LCC, UCC, RTC, LTC, LCB, DCC, RBC, BTL, BRB, BCL, BLB, BCR, BTC, BCB, BCC, BTR};
        return make_pair(nonRotating, rotating);
    }
    pair<vector<int>, vector<int>> BMove()
    {
        vector<int> rotating =  {BTL, BRB, BCL, BLB, BCR, BTC, BCB, BCC, BTR};
        vector<int> nonRotating =  {RCC, LCC, UCC, RTC, LTC, LCB, DCC, RBC, FCR, FCL, FTC, FBC, FTL, FBL, FBR, FCC, FTR};
        return make_pair(nonRotating, rotating);
    }    
    pair<vector<int>, vector<int>> NoMove()
    {
        vector<int> rotating =  {};
        vector<int> nonRotating =  {BTL, BRB, BCL, BLB, BCR, BTC, BCB, BCC, BTR, RCC, LCC, UCC, RTC, LTC, LCB, DCC, RBC, FCR, FCL, FTC, FBC, FTL, FBL, FBR, FCC, FTR};
        return make_pair(nonRotating, rotating);
    }    
};

#endif