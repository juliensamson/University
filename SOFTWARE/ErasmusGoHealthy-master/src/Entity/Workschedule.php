<?php

namespace App\Entity;

use Doctrine\ORM\Mapping as ORM;

/**
 * Workschedule
 * 
 * @ORM\Table(name="workschedule")
 * @ORM\Entity(repositoryClass="App\Repository\WorkscheduleRepository")
 */
class Workschedule
{
    /**
     * @var \DateTime
     *
     * @ORM\Column(name="EndDate", type="datetime", nullable=false, options={"comment"="Date till when the working schedule is valid"})
     */
    private $enddate;

    /**
     * @var int
     *
     * @ORM\Column(name="HoursPerWeek", type="integer", nullable=false, options={"comment"="Defines how much hours per week employee has to work"})
     */
    private $hoursperweek;

    /**
     * @var \DateTime
     *
     * @ORM\Column(name="StartDate", type="datetime", nullable=false, options={"comment"="Date from which the working schedule becomes valid"})
     */
    private $startdate;

    /**
     * @var int
     *
     * @ORM\Column(name="WorkScheduleID", type="integer", nullable=false)
     * @ORM\Id
     * @ORM\GeneratedValue(strategy="IDENTITY")
     */
    private $workscheduleid;

    /**
     * @ORM\Column(name="HoursPerDay", type="integer", nullable=false)
     */
    private $hoursperday;

    public function getEnddate(): ?\DateTimeInterface
    {
        return $this->enddate;
    }

    public function setEnddate(\DateTimeInterface $enddate): self
    {
        $this->enddate = $enddate;

        return $this;
    }

    public function getHoursperweek(): ?int
    {
        return $this->hoursperweek;
    }

    public function setHoursperweek(int $hoursperweek): self
    {
        $this->hoursperweek = $hoursperweek;

        return $this;
    }

    public function getStartdate(): ?\DateTimeInterface
    {
        return $this->startdate;
    }

    public function setStartdate(\DateTimeInterface $startdate): self
    {
        $this->startdate = $startdate;

        return $this;
    }

    public function getWorkscheduleid(): ?int
    {
        return $this->workscheduleid;
    }

    public function getHoursperday(): ?int
    {
        return $this->hoursperday;
    }

    public function setHoursperday(int $hoursperday): self
    {
        $this->hoursperday = $hoursperday;

        return $this;
    }


}

