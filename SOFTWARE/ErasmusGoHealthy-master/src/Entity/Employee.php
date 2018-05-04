<?php

namespace App\Entity;

use Doctrine\ORM\Mapping as ORM;

/**
 * Employee
 *
 * @ORM\Table(name="employee", indexes={@ORM\Index(name="FK_Employee_Has_assigned", columns={"WorkScheduleID"}), @ORM\Index(name="FK_Employee_Can_be", columns={"PersonID"})})
 * @ORM\Entity(repositoryClass="App\Repository\EmployeeRepository")
 */
class Employee
{
    /**
     * @var int|null
     *
     * @ORM\Column(name="WorkScheduleID", type="integer", nullable=true)
     */
    private $workscheduleid;

    /**
     * @var int
     *
     * @ORM\Column(name="EmployeeID", type="integer", nullable=false)
     * @ORM\Id
     * @ORM\GeneratedValue(strategy="IDENTITY")
     */
    private $employeeid;

    /**
     * @var int
     *
     * @ORM\Column(name="PersonID", type="integer", nullable=false)
     */
    private $personid;

    public function getWorkscheduleid(): ?int
    {
        return $this->workscheduleid;
    }

    public function setWorkscheduleid(?int $workscheduleid): self
    {
        $this->workscheduleid = $workscheduleid;

        return $this;
    }

    public function getEmployeeid(): ?int
    {
        return $this->employeeid;
    }

    public function getPersonid(): ?int
    {
        return $this->personid;
    }

    public function setPersonid(int $personid): self
    {
        $this->personid = $personid;

        return $this;
    }


}
