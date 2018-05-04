<?php

namespace App\Entity;

use Doctrine\ORM\Mapping as ORM;

/**
 * @ORM\Entity(repositoryClass="App\Repository\DoctorRepository")
 * @ORM\Table(name="doctor", indexes={@ORM\Index(name="FK_Doctor_Has", columns={"WorkScheduleID"}), @ORM\Index(name="FK_Doctor_Can_be", columns={"PersonID"})})
 */
class Doctor
{
    /**
     * @ORM\Id()
     * @ORM\GeneratedValue(strategy="IDENTITY")
     * @ORM\Column(name="DoctorID", type="integer", nullable=false)
     */
    private $doctorid;

    /**
     * @ORM\Column(name="Specialization", type="string", length=255, nullable=false)
     */
    private $specialization;

    /**
     * @ORM\Column(name="PersonID", type="integer", nullable=false)
     */
    private $personid;

    /**
     * @ORM\Column(name="WorkScheduleID", type="integer", nullable=true)
     */
    private $workscheduleid;

    public function getSpecialization(): ?string
    {
        return $this->specialization;
    }

    public function setSpecialization(string $specialization): self
    {
        $this->specialization = $specialization;

        return $this;
    }

    public function getWorkscheduleid(): ?int
    {
        return $this->workscheduleid;
    }

    public function setWorkscheduleid(?int $workscheduleid): self
    {
        $this->workscheduleid = $workscheduleid;

        return $this;
    }

    public function getDoctorid(): ?int
    {
        return $this->doctorid;
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
