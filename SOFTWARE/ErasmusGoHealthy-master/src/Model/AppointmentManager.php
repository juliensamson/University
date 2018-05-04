<?php

namespace App\Model;

use App\Entity\Appointment;
use Doctrine\ORM\EntityManager;

class AppointmentManager
{
    /**
     * @var EntityManager
     */
    private $entityManager;

    /**
     * AppointmentManager constructor.
     * @param EntityManager $entityManager
     */
    public function __construct(EntityManager $entityManager)
    {
        $this->entityManager = $entityManager;
    }

    public function findAllAppointments(): array
    {
        $appointments = $this->entityManager
        ->getRepository(Appointment::class)
        ->findAll();

        return $appointments;
    }

    public function findAppointmentById($id): Appointment
    {
        $appointment = $this->entityManager
        ->getRepository(Appointment::class)
        ->findById($id);

        return $appointment;
    }

    public function findPatientsAppoitments($patientid): array
    {
        $appointments = $this->entityManager
        ->getRepository(Appointment::class)
        ->findByPatientId($patientid);

        return $appointments;
    }

    public function findDoctorsAppoitments($doctorid): array
    {
        $appointments = $this->entityManager
        ->getRepository(Appointment::class)
        ->findByDoctorId($patientid);

        return $appointments;
    }

}
?>
