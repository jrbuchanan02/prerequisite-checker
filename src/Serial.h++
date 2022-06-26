#pragma once

#include <istream>
#include <memory>

#include <Extracted.h++>
#include <Keywords.h++>

class Serial
{
protected:
    static inline bool isCorrectType ( std::string          type,
                                       ExtractedItem const &item )
    {
        using namespace keywords;
        auto position =
                std::find_if ( item.begin ( ), item.end ( ), [ & ] ( auto t ) {
                    return t.key == keys::type and t.val == type;
                } );
        return position != item.end ( );
    }

    static inline std::string
            wrongTypeErrorMessage ( std::string          type,
                                    ExtractedItem const &item )
    {
        using namespace keywords;
        std::string errorMessage = "Found erroneous type. Expected ";
        errorMessage += type;
        errorMessage += " but found ";
        // find the type.
        auto position =
                std::find_if ( item.begin ( ), item.end ( ), [] ( auto t ) {
                    return t.key == keys::type;
                } );
        if ( position == item.end ( ) )
        {
            errorMessage += "an item without a type!";
        } else
        {
            errorMessage += position->val;
        }
        return errorMessage;
    }

    static inline void throwOnWrongType ( std::string          type,
                                          ExtractedItem const &item )
    {
        if ( not isCorrectType ( type, item ) )
        {
            std::string errorMessage = wrongTypeErrorMessage ( type, item );
            throw std::runtime_error ( errorMessage );
        }
    }
public:
    Serial ( ) noexcept                = default;
    Serial ( Serial const & ) noexcept = default;
    Serial ( Serial && ) noexcept      = default;
    virtual ~Serial ( )                = default;

    Serial &operator= ( Serial const & ) noexcept = default;
    Serial &operator= ( Serial && ) noexcept      = default;

    virtual void extract ( ExtractedItem const & ) = 0;
};