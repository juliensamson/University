<?php

namespace App\Entity;

use Doctrine\ORM\Mapping as ORM;

/**
 * Healthrecord
 *
 * @ORM\Table(name="healthrecord", indexes={@ORM\Index(name="FK_Health_record_Records", columns={"DoctorID"}), @ORM\Index(name="FK_Health_record_Has", columns={"PatientID"})})
 * @ORM\Entity
 */
class Healthrecord
{
    /**
     * @var int|null
     *
     * @ORM\Column(name="CodeOfIllness", type="integer", nullable=true)
     */
    private $codeofillness;

    /**
     * @var \DateTime
     *
     * @ORM\Column(name="Date", type="datetime", nullable=false)
     */
    private $date;

    /**
     * @var string
     *
     * @ORM\Column(name="Description", type="text", length=65535, nullable=false)
     */
    private $description;

    /**
     * @var int
     *
     * @ORM\Column(name="HealthRecordID", type="integer", nullable=false)
     * @ORM\Id
     * @ORM\GeneratedValue(strategy="IDENTITY")
     */
    private $healthrecordid;

    /**
     * @var int
     *
     * @ORM\Column(name="DoctorID", type="integer", nullable=false)
     */
    private $doctorid;

    /**
     * @var int
     *
     * @ORM\Column(name="PatientID", type="integer", nullable=false)
     */
    private $patientid;

    public function getCodeofillness(): ?int
    {
        return $this->codeofillness;
    }

    public function setCodeofillness(?int $codeofillness): self
    {
        $this->codeofillness = $codeofillness;

        return $this;
    }

    public function getDate(): ?\DateTimeInterface
    {
        return $this->date;
    }

    public function setDate(\DateTimeInterface $date): self
    {
        $this->date = $date;

        return $this;
    }

    public function getDescription(): ?string
    {
        return $this->description;
    }

    public function setDescription(string $description): self
    {
        $this->description = $description;

        return $this;
    }

    public function getHealthrecordid(): ?int
    {
        return $this->healthrecordid;
    }

    public function getDoctorid(): ?int
    {
        return $this->doctorid;
    }

    public function setDoctorid(int $doctorid): self
    {
        $this->doctorid = $doctorid;

        return $this;
    }

    public function getPatientid(): ?int
    {
        return $this->patientid;
    }

    public function setPatientid(int $patientid): self
    {
        $this->patientid = $patientid;

        return $this;
    }


}
