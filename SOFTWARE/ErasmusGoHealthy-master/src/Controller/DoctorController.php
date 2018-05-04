<?php

namespace App\Controller;

use App\Entity\Doctor;
use App\Form\DoctorType;
use App\Repository\DoctorRepository;
use Symfony\Bundle\FrameworkBundle\Controller\Controller;
use Symfony\Component\HttpFoundation\Request;
use Symfony\Component\HttpFoundation\Response;
use Symfony\Component\Routing\Annotation\Route;

/**
 * @Route("/doctor")
 */
class DoctorController extends Controller
{
    /**
     * @Route("/", name="doctor_index", methods="GET")
     */
    public function index(DoctorRepository $doctorRepository): Response
    {
        return $this->render('doctor/index.html.twig', ['doctors' => $doctorRepository->findAll()]);
    }

    /**
     * @Route("/specializations", name="specializations", methods="GET")
     */
    public function specializations(DoctorRepository $doctorRepository): Response
    {
        return $this->render('doctor/specializations.html.twig', ['doctors' => $doctorRepository->findAllSpecializations()]);
    }

    /**
     * @Route("/specializations/{specialization}", name="specialists", methods="GET")
     */
    public function specialists(DoctorRepository $doctorRepository, $specialization): Response
    {
        return $this->render('doctor/specialists.html.twig', ['doctors' => $doctorRepository->findBySpecialization($specialization)]);
    }


    /**
     * @Route("/new", name="doctor_new", methods="GET|POST")
     */
    public function new(Request $request): Response
    {
        $doctor = new Doctor();
        $form = $this->createForm(DoctorType::class, $doctor);
        $form->handleRequest($request);

        if ($form->isSubmitted() && $form->isValid()) {
            $em = $this->getDoctrine()->getManager();
            $em->persist($doctor);
            $em->flush();

            return $this->redirectToRoute('doctor_index');
        }

        return $this->render('doctor/new.html.twig', [
            'doctor' => $doctor,
            'form' => $form->createView(),
        ]);
    }

    /**
     * @Route("/{doctorid}", name="doctor_show", methods="GET")
     */
    public function show(Doctor $doctor): Response
    {
        return $this->render('doctor/show.html.twig', ['doctor' => $doctor]);
    }

    /**
     * @Route("/{doctorid}/edit", name="doctor_edit", methods="GET|POST")
     */
    public function edit(Request $request, Doctor $doctor): Response
    {
        $form = $this->createForm(DoctorType::class, $doctor);
        $form->handleRequest($request);

        if ($form->isSubmitted() && $form->isValid()) {
            $this->getDoctrine()->getManager()->flush();

            return $this->redirectToRoute('doctor_edit', ['doctorid' => $doctor->getDoctorid()]);
        }

        return $this->render('doctor/edit.html.twig', [
            'doctor' => $doctor,
            'form' => $form->createView(),
        ]);
    }

    /**
     * @Route("/{doctorid}", name="doctor_delete", methods="DELETE")
     */
    public function delete(Request $request, Doctor $doctor): Response
    {
        if ($this->isCsrfTokenValid('delete'.$doctor->getDoctorid(), $request->request->get('_token'))) {
            $em = $this->getDoctrine()->getManager();
            $em->remove($doctor);
            $em->flush();
        }

        return $this->redirectToRoute('doctor_index');
    }
}
