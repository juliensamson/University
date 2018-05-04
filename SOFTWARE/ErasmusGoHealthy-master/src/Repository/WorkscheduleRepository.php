<?php

namespace App\Repository;

use App\Entity\Workschedule;
use Doctrine\Bundle\DoctrineBundle\Repository\ServiceEntityRepository;
use Symfony\Bridge\Doctrine\RegistryInterface;

/**
 * @method Workschedule|null find($id, $lockMode = null, $lockVersion = null)
 * @method Workschedule|null findOneBy(array $criteria, array $orderBy = null)
 * @method Workschedule[]    findAll()
 * @method Workschedule[]    findBy(array $criteria, array $orderBy = null, $limit = null, $offset = null)
 */
class WorkscheduleRepository extends ServiceEntityRepository
{
    public function __construct(RegistryInterface $registry)
    {
        parent::__construct($registry, Workschedule::class);
    }

//    /**
//     * @return Workschedule[] Returns an array of Workschedule objects
//     */
    /*
    public function findByExampleField($value)
    {
        return $this->createQueryBuilder('w')
            ->andWhere('w.exampleField = :val')
            ->setParameter('val', $value)
            ->orderBy('w.id', 'ASC')
            ->setMaxResults(10)
            ->getQuery()
            ->getResult()
        ;
    }
    */

    /*
    public function findOneBySomeField($value): ?Workschedule
    {
        return $this->createQueryBuilder('w')
            ->andWhere('w.exampleField = :val')
            ->setParameter('val', $value)
            ->getQuery()
            ->getOneOrNullResult()
        ;
    }
    */
}
