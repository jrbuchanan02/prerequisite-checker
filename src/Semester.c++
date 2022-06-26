#include <Extracted.h++>
#include <Keywords.h++>
#include <Semester.h++>

#include <istream>
#include <sstream>
#include <string>
#include <vector>

using keywords::keys::tagged;
using keywords::tags::uncheckedSemester;
using keywords::types::semester;

void Semester::extract ( ExtractedItem const &extracted )
{
    throwOnWrongType ( semester, extracted );
    Referred::extract ( extracted );

    // Semester only cares about being tagged "unchecked", if so,
    // we add that flag.
    auto tags = filterForTagType ( extracted, tagged );
    auto unchecked =
            std::find_if ( tags.begin ( ), tags.end ( ), [] ( auto t ) {
                return t.val == uncheckedSemester;
            } );
    if ( unchecked != tags.end ( ) )
    {
        addFlag ( uncheckedSemester );
    }
}

bool const Semester::isChecked ( ) const noexcept
{
    return not hasFlag ( uncheckedSemester );
}