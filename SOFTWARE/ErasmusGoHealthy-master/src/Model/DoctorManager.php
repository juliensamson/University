<?php

namespace App\Model;

use App\Entity\Doctor;
use Doctrine\ORM\EntityManager;

class DoctorManager 
{
    /**
     * @var EntityManager
     */
    private $entityManager;

    /**
     * DoctorManager constructor.
     * @param EntityManager $entityManager
     */
    public function __construct(EntityManager $entityManager)
    {
        $this->entityManager = $entityManager;
    }


    public function GetAllDoctors(): array
    {
        $doctors = $this->entityManager
        ->getRepository(Doctor::class)
        ->findAll();

        return $doctors;
    }

    public function GetAllSpecializations(){
        
    }
}