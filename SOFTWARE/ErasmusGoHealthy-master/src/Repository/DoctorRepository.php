<?php

namespace App\Repository;

use App\Entity\Doctor;
use Doctrine\Bundle\DoctrineBundle\Repository\ServiceEntityRepository;
use Symfony\Bridge\Doctrine\RegistryInterface;

/**
 * @method Doctor|null find($id, $lockMode = null, $lockVersion = null)
 * @method Doctor|null findOneBy(array $criteria, array $orderBy = null)
 * @method Doctor[]    findAll()
 * @method Doctor[]    findBy(array $criteria, array $orderBy = null, $limit = null, $offset = null)
 */
class DoctorRepository extends ServiceEntityRepository
{
    public function __construct(RegistryInterface $registry)
    {
        parent::__construct($registry, Doctor::class);
    }

    /**
     * @return Doctor[] Returns an array of Doctor objects grouped by
     * specializations
     */
    public function findAllSpecializations()
    {
        return $this->createQueryBuilder('d')
            ->groupBy('d.specialization')
            ->orderBy('d.specialization', 'ASC')
            ->getQuery()
            ->getResult()
        ;
    }

    /**
     * @return Doctor[] Returns an array of Full Doctor objects
     */
    public function findBySpecialization($specialization): array
    {
        $entityManager = $this->getEntityManager();

        $query = $entityManager->createQuery(
            'SELECT d.doctorid, d.specialization, p.name, p.surname
            FROM App\Entity\Doctor d, App\Entity\Person p
            WHERE d.specialization = :spec
            AND d.personid = p.personid
            ORDER BY p.surname ASC'
        )->setParameter('spec', $specialization);
    
        // returns an array of Full Doctor objects
        return $query->execute();
    }

    /*
    public function findOneBySomeField($value): ?Doctor
    {
        return $this->createQueryBuilder('d')
            ->andWhere('d.exampleField = :val')
            ->setParameter('val', $value)
            ->getQuery()
            ->getOneOrNullResult()
        ;
    }
    */
}
