/**
 * @file Keywords.h++
 * @author Joshua Buchanan (joshuarobertbuchanan@gmail.com)
 * @brief The keywords.
 * @version 1
 * @date 2022-06-25
 *
 * @copyright Copyright (C) 2022. Intellectual property of the author(s) listed
 * above.
 *
 */

#pragma once

#include <concepts>
#include <functional>
#include <string>
#include <vector>

namespace keywords
{
    namespace keys
    {
        static inline std::string type         = "type";
        static inline std::string ref          = "ref";
        static inline std::string name         = "name";
        static inline std::string desc         = "desc";
        static inline std::string hours        = "hours";
        static inline std::string reqs         = "reqs";
        static inline std::string req          = "req";
        static inline std::string tagged       = "tagged";
        static inline std::string comment      = "#";
        static inline std::string courses      = "courses";
        static inline std::string requisiteses = "requisites";
        static inline std::string plans        = "plans";
        static inline std::string semesters    = "semesters";
    } // namespace keys

    namespace prefixes
    {
        static inline std::string begin = "";
        static inline std::string end   = "end";
    } // namespace prefixes

    namespace types
    {
        static inline std::string course     = "course";
        static inline std::string requisites = "reqs";
        static inline std::string semester   = "semester";
        static inline std::string plan       = "plan";
        static inline std::string manifest   = "manifest";

        std::vector< std::string > const &types ( ) noexcept;
        std::vector< std::string > const &begins ( ) noexcept;
        std::vector< std::string > const &ends ( ) noexcept;

        std::string beginOf ( std::string const & );
        std::string endOf ( std::string const & );

        std::vector< std::string >::const_iterator typesBegin ( ) noexcept;
        std::vector< std::string >::const_iterator typesEnd ( ) noexcept;
        std::vector< std::string >::const_iterator beginsBegin ( ) noexcept;
        std::vector< std::string >::const_iterator beginsEnd ( ) noexcept;
        std::vector< std::string >::const_iterator endsBegin ( ) noexcept;
        std::vector< std::string >::const_iterator endsEnd ( ) noexcept;

        inline auto typesFind ( ) noexcept
        {
            using namespace std::placeholders;
            auto find = std::find< decltype ( typesBegin ( ) ),
                                   decltype ( types ( ).at ( 0 ) ) >;
            return std::bind ( find, typesBegin ( ), typesEnd ( ), _1 );
        }

        inline auto beginsFind ( ) noexcept
        {
            using namespace std::placeholders;
            auto find = std::find< decltype ( beginsBegin ( ) ),
                                   decltype ( begins ( ).at ( 0 ) ) >;
            return std::bind ( find, beginsBegin ( ), beginsEnd ( ), _1 );
        }

        inline auto endsFind ( ) noexcept
        {
            using namespace std::placeholders;
            auto find = std::find< decltype ( endsBegin ( ) ),
                                   decltype ( ends ( ).at ( 0 ) ) >;
            return std::bind ( find, endsBegin ( ), endsEnd ( ), _1 );
        }
    } // namespace types

    namespace tags
    {
        static inline std::string takenPreviously   = "pre";
        static inline std::string takenConcurrent   = "con";
        static inline std::string uncheckedSemester = "unchecked";
    } // namespace tags

    namespace delimiters
    {
        static inline std::string requisiteCourseDelimiter   = "or";
        static inline std::string planCourseDelimiter        = "and";
        static inline std::string planSemesterDelimiter      = ":";
        static inline std::string commandlineCourseDelimiter = ",";
        static inline std::string statementDelimiter         = "\n";
        static inline std::string space                      = " ";
    } // namespace delimiters

    namespace yaml
    {
        static inline std::string requisiteID   = "id";
        static inline std::string manifestEntry = keys::courses;
    } // namespace yaml

    namespace manipulators
    {
        inline std::string removeLeading ( std::string const &phrase,
                                           std::string const &from )
        {
            if ( from.starts_with ( phrase ) )
            {
                return from.substr ( phrase.size ( ) );
            } else
            {
                return from;
            }
        }

        inline std::string removeTrailing ( std::string const &phrase,
                                            std::string const &from )
        {
            if ( from.ends_with ( phrase ) )
            {
                return from.substr ( 0, from.size ( ) - phrase.size ( ) );
            } else
            {
                return from;
            }
        }

        inline std::string removeAllLeading ( std::string const &phrase,
                                              std::string const &from )
        {
            std::string result;
            for ( result = from; result != removeLeading ( phrase, result );
                  result = removeLeading ( phrase, result ) )
            { }
            return result;
        }

        inline std::string removeAllTrailing ( std::string const &phrase,
                                               std::string const &from )
        {
            std::string result;
            for ( result = from; result != removeTrailing ( phrase, result );
                  result = removeTrailing ( phrase, result ) )
            { }
            return result;
        }

        inline std::string removeAllLeading ( std::input_iterator auto begin,
                                              std::input_iterator auto end,
                                              std::string const       &from )
        {
            std::string result     = from;
            bool        madeChange = true;
            while ( madeChange )
            {
                madeChange = false;
                for ( auto phraseIterator = begin; phraseIterator != end;
                      phraseIterator++ )
                {
                    std::string previous = result;
                    result     = removeAllLeading ( *phraseIterator, result );
                    madeChange = madeChange bitor ( result != previous );
                }
            }
            return result;
        }

        inline std::string removeAllTrailing ( std::input_iterator auto begin,
                                               std::input_iterator auto end,
                                               std::string const       &from )
        {
            std::string result     = from;
            bool        madeChange = true;
            while ( madeChange )
            {
                madeChange = false;
                for ( auto phraseIterator = begin; phraseIterator != end;
                      phraseIterator++ )
                {
                    std::string previous = result;
                    result     = removeAllTrailing ( *phraseIterator, result );
                    madeChange = madeChange bitor ( result != previous );
                }
            }
            return result;
        }

        inline bool eitherEqual ( std::equality_comparable auto const &first,
                                  std::equality_comparable auto const &second,
                                  std::equality_comparable auto const &with )
        {
            return first == with or second == with;
        }

        inline bool contains ( std::input_iterator auto      begin,
                               std::input_iterator auto      end,
                               std::equality_comparable auto token )
        {
            return std::find ( begin, end, token ) != end;
        }
    } // namespace manipulators
} // namespace keywords