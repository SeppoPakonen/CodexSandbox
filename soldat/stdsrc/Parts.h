#ifndef PARTS_H
#define PARTS_H

//*******************************************************************************
//                                                                              
//       Particle Unit for SOLDAT                                               
//                                                                              
//       Copyright (c) 2001-02 Michal Marcinkowski      
//       PARTS ver. 1.0.7                                                       
//       PARTICLE & CONSTRAINT PHYSICS MODULE FOR DELPHI 2D                   
//        by Michal Marcinkowski                                                
//                                                                              
//        version history:                                                      
//        Current - Modified for SOLDAT                                         
//        1.0.7 - Changed to 2D                                                 
//        1.0.6 - Added Constraint Active variable                              
//        1.0.5 - Added PO Constraints only loading                             
//        1.0.4 - Changed to D3DX Vectors                                       
//        1.0.3 - Added PO loader scaling                                       
//        1.0.2 - Added Euler Integrator                                        
//        1.0.1 - Added PO files loading                                        
//                                                                              
//*******************************************************************************

#include "Vector.h"
#include <cmath>
#include <cstring>

const int NUM_PARTICLES = 560;
const float RKV = 0.98f;

struct Constraint {
    bool Active;
    int PartA, PartB;
    float Restlength;
};

struct ParticleSystem {
    bool Active[NUM_PARTICLES + 1];  // Pascal arrays start from 1
    TVector2 Pos[NUM_PARTICLES + 1];
    TVector2 Velocity[NUM_PARTICLES + 1];
    TVector2 OldPos[NUM_PARTICLES + 1];
    TVector2 Forces[NUM_PARTICLES + 1];
    float OneOverMass[NUM_PARTICLES + 1];
    float TimeStep;
    float Gravity, VDamping, EDamping;
    int ConstraintCount;
    int PartCount;
    Constraint Constraints[NUM_PARTICLES + 1];  // Pascal arrays start from 1

    ParticleSystem() : TimeStep(1.0f), Gravity(0.0f), VDamping(0.99f), EDamping(0.99f), 
                       ConstraintCount(0), PartCount(0) {
        // Initialize arrays
        for (int i = 0; i <= NUM_PARTICLES; i++) {
            Active[i] = false;
            Pos[i] = {0.0f, 0.0f};
            Velocity[i] = {0.0f, 0.0f};
            OldPos[i] = {0.0f, 0.0f};
            Forces[i] = {0.0f, 0.0f};
            OneOverMass[i] = 0.0f;
            if (i > 0) {
                Constraints[i] = {false, 0, 0, 0.0f};
            }
        }
    }

    void DoVerletTimeStep();
    void DoVerletTimeStepFor(int I, int J);
    void DoEulerTimeStep();
    void DoEulerTimeStepFor(int I);
    void CreatePart(TVector2 Start, TVector2 Vel, float Mass, int Num);
    void MakeConstraint(int PA, int PB, float Rest);
    void Clone(const ParticleSystem& Other);
    void LoadPOObject(const std::string& Filename, float Scale);
    void StopAllParts();
    void Destroy();
    void SatisfyConstraints();

private:
    void Verlet(int I);
    void Euler(int I);
    void SatisfyConstraintsFor(int I);
};

namespace PartsImpl {
    inline void ParticleSystem::DoVerletTimeStep() {
        for (int i = 1; i <= NUM_PARTICLES; i++) {
            if (Active[i]) {
                Verlet(i);
            }
        }
        SatisfyConstraints();
    }

    inline void ParticleSystem::DoVerletTimeStepFor(int I, int J) {
        Verlet(I);
        SatisfyConstraintsFor(J);
    }

    inline void ParticleSystem::DoEulerTimeStepFor(int I) {
        Euler(I);
    }

    inline void ParticleSystem::DoEulerTimeStep() {
        for (int i = 1; i <= NUM_PARTICLES; i++) {
            if (Active[i]) {
                Euler(i);
            }
        }
    }

    inline void ParticleSystem::Euler(int I) {
        // Accumulate Forces
        Forces[I].y += Gravity;
        TVector2 tempPos = Pos[I];

        TVector2 S = Vec2Scale(Forces[I], OneOverMass[I]);
        S = Vec2Scale(S, TimeStep * TimeStep);

        Velocity[I] = Vec2Add(Velocity[I], S);
        Pos[I] = Vec2Add(Pos[I], Velocity[I]);
        Velocity[I] = Vec2Scale(Velocity[I], EDamping);
        OldPos[I] = tempPos;

        Forces[I].x = 0.0f;
        Forces[I].y = 0.0f;
    }

    inline void ParticleSystem::Verlet(int I) {
        // Accumulate Forces
        Forces[I].y += Gravity;
        TVector2 tempPos = Pos[I];

        // Pos[I] := 2 * Pos[I] - OldPos[I] + Forces[I]{ / Mass} * TimeStep * TimeStep;  {Verlet integration}
        TVector2 S1 = Vec2Scale(Pos[I], 1.0f + VDamping);
        TVector2 S2 = Vec2Scale(OldPos[I], VDamping);

        TVector2 D = Vec2Subtract(S1, S2);
        S1 = Vec2Scale(Forces[I], OneOverMass[I]);
        S2 = Vec2Scale(S1, TimeStep * TimeStep);

        Pos[I] = Vec2Add(D, S2);
        OldPos[I] = tempPos;

        Forces[I].x = 0.0f;
        Forces[I].y = 0.0f;
    }

    inline void ParticleSystem::SatisfyConstraints() {
        if (ConstraintCount > 0) {
            for (int i = 1; i <= ConstraintCount; i++) {
                Constraint& constraint = Constraints[i];
                if (constraint.Active) {
                    float diff = 0.0f;
                    TVector2 delta = Vec2Subtract(Pos[constraint.PartB], Pos[constraint.PartA]);
                    float deltaLength = std::sqrt(Vec2Dot(delta, delta));
                    if (deltaLength != 0.0f) {
                        diff = (deltaLength - constraint.Restlength) / deltaLength;
                    }
                    if (OneOverMass[constraint.PartA] > 0) {
                        TVector2 d = Vec2Scale(delta, 0.5f * diff);
                        Pos[constraint.PartA] = Vec2Add(Pos[constraint.PartA], d);
                    }
                    if (OneOverMass[constraint.PartB] > 0) {
                        TVector2 d = Vec2Scale(delta, 0.5f * diff);
                        Pos[constraint.PartB] = Vec2Subtract(Pos[constraint.PartB], d);
                    }
                }
            }
        }
    }

    inline void ParticleSystem::SatisfyConstraintsFor(int I) {
        Constraint& constraint = Constraints[I];
        float diff = 0.0f;
        TVector2 delta = Vec2Subtract(Pos[constraint.PartB], Pos[constraint.PartA]);
        float deltaLength = std::sqrt(Vec2Dot(delta, delta));
        if (deltaLength != 0.0f) {
            diff = (deltaLength - constraint.Restlength) / deltaLength;
        }
        if (OneOverMass[constraint.PartA] > 0) {
            TVector2 d = Vec2Scale(delta, 0.5f * diff);
            Pos[constraint.PartA] = Vec2Add(Pos[constraint.PartA], d);
        }
        if (OneOverMass[constraint.PartB] > 0) {
            TVector2 d = Vec2Scale(delta, 0.5f * diff);
            Pos[constraint.PartB] = Vec2Subtract(Pos[constraint.PartB], d);
        }
    }

    inline void ParticleSystem::CreatePart(TVector2 Start, TVector2 Vel, float Mass, int Num) {
        // Num is now the active Part
        Active[Num] = true;
        Pos[Num] = Start;
        Velocity[Num] = Vel;
        OldPos[Num] = Start;
        OneOverMass[Num] = 1.0f / Mass;
    }

    inline void ParticleSystem::MakeConstraint(int PA, int PB, float Rest) {
        ConstraintCount++;
        Constraint& constraint = Constraints[ConstraintCount];
        constraint.Active = true;
        constraint.PartA = PA;
        constraint.PartB = PB;
        constraint.Restlength = Rest;
    }

    inline void ParticleSystem::Clone(const ParticleSystem& Other) {
        ConstraintCount = Other.ConstraintCount;
        PartCount = Other.PartCount;

        std::memcpy(Active, Other.Active, sizeof(Active));
        std::memcpy(Pos, Other.Pos, sizeof(Pos));
        std::memcpy(Velocity, Other.Velocity, sizeof(Velocity));
        std::memcpy(OldPos, Other.OldPos, sizeof(OldPos));
        std::memcpy(OneOverMass, Other.OneOverMass, sizeof(OneOverMass));
        std::memcpy(Forces, Other.Forces, sizeof(Forces));

        for (int i = 1; i <= ConstraintCount; i++) {
            const Constraint& otherConstraint = Other.Constraints[i];
            Constraint& constraint = Constraints[i];
            constraint.Active = otherConstraint.Active;
            constraint.PartA = otherConstraint.PartA;
            constraint.PartB = otherConstraint.PartB;
            constraint.Restlength = otherConstraint.Restlength;
        }
    }

    inline void ParticleSystem::LoadPOObject(const std::string& Filename, float Scale) {
        // This function would load a PO (particle object) file
        // It requires PHYSFS functionality which would need to be implemented or linked
        // For now, this is a stub implementation
    }

    inline void ParticleSystem::StopAllParts() {
        for (int i = 1; i <= NUM_PARTICLES; i++) {
            if (Active[i]) {
                Velocity[i].x = 0.0f;
                Velocity[i].y = 0.0f;
                OldPos[i] = Pos[i];
            }
        }
    }

    inline void ParticleSystem::Destroy() {
        for (int i = 1; i <= NUM_PARTICLES; i++) {
            Active[i] = false;
            Pos[i].x = 0.0f;
            Pos[i].y = 0.0f;
            OldPos[i] = Pos[i];
            Velocity[i].x = 0.0f;
            Velocity[i].y = 0.0f;
            Forces[i].x = 0.0f;
            Forces[i].y = 0.0f;
            OneOverMass[i] = 0.0f;
        }
        ConstraintCount = 0;
    }
}

// Using declarations to bring into global namespace
using PartsImpl::Constraint;
using PartsImpl::ParticleSystem;

#endif // PARTS_H