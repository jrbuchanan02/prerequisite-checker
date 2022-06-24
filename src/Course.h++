/**
 * \file Course.h++
 * \brief Defines what a Course is
 * \author Joshua Buchanan
 * \date 2021-11-01 (file creation) 2021-11-02 (last modified / documented)
 * \version 1
 * \copyright Joshua Buchanan (C) 2021; MIT License
 */
#pragma once

#include <istream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

class Course;

using CoursePointer = std::shared_ptr<Course>;

#include <Flagged.h++>
#include <Reference.h++>
#include <Referred.h++>
#include <Registry.h++>
#include <Requisites.h++>

/**
 * \class Course
 * \brief A class taken in a University. Courses have multiple Requisites that
 * all must be met for the course to have its requisites met. In addition to a
 * unique reference, Course also has a human-readable name and description (ex,
 * CS 101 is CS II for Majors, and has a paragraph-length description). A course
 * provides a certain number of hours that have certain flags.
 * Therefore, a course is both Flagged and Referred.
 */
class Course : public Flagged, public Referred
{
    double hours = -1;
    std::vector<Reference> requisites;
    std::string name = "";
    std::string desc = "";

public:
    /**
     * \brief Explicitly tell g++ that Course can be constructed by default and
     * trivially so.
     */
    inline Course() noexcept = default;
    /**
     * \brief Explicitly tell g++ that Course can be trivially copied without
     * ever erroring out.
     */
    inline Course(Course const &) noexcept = default;
    /**
     * \brief Explicitly tell g++ that Course can be trivially moved without
     * ever erroring out.
     */
    inline Course(Course &&) noexcept = default;
    /**
     * \brief Constructs a Course given all possibly necesssary information
     * \param flags the flags which apply to this course, for example, Honors courses
     * would have the honors flag
     * \param reference this Course's reference
     * \param requisites this Course's Requisites
     * \param name the human readable name of this course
     * \param desc the description for this course.
     */
    Course(
        std::vector<std::string> const &flags,
        Reference const &reference,
        double const &hours,
        std::vector<Reference> const &requisites,
        std::string const &name,
        std::string const &desc) noexcept;
    /**
     * \brief Explicitly tell g++ that it knows how to destruct a Course
     */
    inline virtual ~Course() = default;

    /**
     * \brief gets the amount of hours this course is worth
     * \return `this->hours`
     */
    double const &getHours() const noexcept;
    /**
     * \brief gets the human readable name of this course
     */
    std::string const &getName() const noexcept;
    /**
     * \brief gets the description of this course
     */
    std::string const &getDesc() const noexcept;
    std::vector<RequisitesPointer> const resolveRequisites(Registry const &) noexcept;
    bool const hoursHaveAttribute(std::string const &) const noexcept;

    bool const meetsRequisites(std::vector<std::vector<Reference>> const &, Registry const &, Reference &);
    /**
     * \brief Tell g++ that if it knows how to copy a Course without error, it then
     * knows how to use the copy-assignment operator without error.
     */
    inline Course &operator=(Course const &) noexcept = default;
    /**
     * \brief Tell g++ that if it knows how to move a Course without error, it then
     * knows how to use the move-assignment operator without error.
     */
    inline Course &operator=(Course &&) noexcept = default;
    
    virtual void extract(ExtractedItem const &) override;
};
