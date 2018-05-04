<?php

namespace App\Repository;

use App\Entity\Appointment;
use Doctrine\Bundle\DoctrineBundle\Repository\ServiceEntityRepository;
use Symfony\Bridge\Doctrine\RegistryInterface;

/**
 * @method Appointment|null find($id, $lockMode = null, $lockVersion = null)
 * @method Appointment|null findOneBy(array $criteria, array $orderBy = null)
 * @method Appointment[]    findAll()
 * @method Appointment[]    findBy(array $criteria, array $orderBy = null, $limit = null, $offset = null)
 */
class AppointmentRepository extends ServiceEntityRepository
{
    public function __construct(RegistryInterface $registry)
    {
        parent::__construct($registry, Appointment::class);
    }

    /**
     * @return Appointment[] Returns an array of Appointment objects     
     */
    public function findByPatientId($patientid)
    {
        return $this->createQueryBuilder('a')
            ->andWhere('a.patientid = :val')
            ->setParameter('val', $patientid)
            ->orderBy('a.date', 'ASC')
            ->setMaxResults(10)
            ->getQuery()
            ->getResult()
        ;
    }

    /**
     * @return Appointment[] Returns an array of Appointment objects     
     */
    public function findByDoctorId($doctorid)
    {
        return $this->createQueryBuilder('a')
            ->andWhere('a.doctorid = :val')
            ->setParameter('val', $doctorid)
            ->orderBy('a.date', 'ASC')
            ->setMaxResults(10)
            ->getQuery()
            ->getResult()
        ;
    }


    /*
    public function findOneBySomeField($value): ?Appointment
    {
        return $this->createQueryBuilder('a')
            ->andWhere('a.exampleField = :val')
            ->setParameter('val', $value)
            ->getQuery()
            ->getOneOrNullResult()
        ;
    }
    */
}
