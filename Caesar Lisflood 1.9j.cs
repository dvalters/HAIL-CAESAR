// CAESAR Lisflood is available under the terms of a GNU licence. see below:
//
//                    GNU GENERAL PUBLIC LICENSE
//                       Version 3, 29 June 2007

// Copyright (C) 2007 Free Software Foundation, Inc. <http://fsf.org/>
// Everyone is permitted to copy and distribute verbatim copies
// of this license document, but changing it is not allowed.

//                            Preamble

//  The GNU General Public License is a free, copyleft license for
//software and other kinds of works

//  The licenses for most software and other practical works are designed
//to take away your freedom to share and change the works.  By contrast,
//the GNU General Public License is intended to guarantee your freedom to
//share and change all versions of a program--to make sure it remains free
//software for all its users.  We, the Free Software Foundation, use the
//GNU General Public License for most of our software; it applies also to
//any other work released this way by its authors.  You can apply it to
//your programs, too.

//  When we speak of free software, we are referring to freedom, not
//price.  Our General Public Licenses are designed to make sure that you
//have the freedom to distribute copies of free software (and charge for
//them if you wish), that you receive source code or can get it if you
//want it, that you can change the software or use pieces of it in new
//free programs, and that you know you can do these things.

//  To protect your rights, we need to prevent others from denying you
//these rights or asking you to surrender the rights.  Therefore, you have
//certain responsibilities if you distribute copies of the software, or if
//you modify it: responsibilities to respect the freedom of others.

//  For example, if you distribute copies of such a program, whether
//gratis or for a fee, you must pass on to the recipients the same
//freedoms that you received.  You must make sure that they, too, receive
//or can get the source code.  And you must show them these terms so they
//know their rights.

//  Developers that use the GNU GPL protect your rights with two steps:
//(1) assert copyright on the software, and (2) offer you this License
//giving you legal permission to copy, distribute and/or modify it.

//  For the developers' and authors' protection, the GPL clearly explains
//that there is no warranty for this free software.  For both users' and
//authors' sake, the GPL requires that modified versions be marked as
//changed, so that their problems will not be attributed erroneously to
//authors of previous versions.

//  Some devices are designed to deny users access to install or run
//modified versions of the software inside them, although the manufacturer
//can do so.  This is fundamentally incompatible with the aim of
//protecting users' freedom to change the software.  The systematic
//pattern of such abuse occurs in the area of products for individuals to
//use, which is precisely where it is most unacceptable.  Therefore, we
//have designed this version of the GPL to prohibit the practice for those
//products.  If such problems arise substantially in other domains, we
//stand ready to extend this provision to those domains in future versions
//of the GPL, as needed to protect the freedom of users.

//  Finally, every program is threatened constantly by software patents.
//States should not allow patents to restrict development and use of
//software on general-purpose computers, but in those that do, we wish to
//avoid the special danger that patents applied to a free program could
//make it effectively proprietary.  To prevent this, the GPL assures that
//patents cannot be used to render the program non-free.

//  The precise terms and conditions for copying, distribution and
//modification follow.

//                       TERMS AND CONDITIONS

//  0. Definitions.

//  "This License" refers to version 3 of the GNU General Public License.

//  "Copyright" also means copyright-like laws that apply to other kinds of
//works, such as semiconductor masks.

//  "The Program" refers to any copyrightable work licensed under this
//License.  Each licensee is addressed as "you".  "Licensees" and
//"recipients" may be individuals or organizations.

//  To "modify" a work means to copy from or adapt all or part of the work
//in a fashion requiring copyright permission, other than the making of an
//exact copy.  The resulting work is called a "modified version" of the
//earlier work or a work "based on" the earlier work.

//  A "covered work" means either the unmodified Program or a work based
//on the Program.

//  To "propagate" a work means to do anything with it that, without
//permission, would make you directly or secondarily liable for
//infringement under applicable copyright law, except executing it on a
//computer or modifying a private copy.  Propagation includes copying,
//distribution (with or without modification), making available to the
//public, and in some countries other activities as well.

//  To "convey" a work means any kind of propagation that enables other
//parties to make or receive copies.  Mere interaction with a user through
//a computer network, with no transfer of a copy, is not conveying.

//  An interactive user interface displays "Appropriate Legal Notices"
//to the extent that it includes a convenient and prominently visible
//feature that (1) displays an appropriate copyright notice, and (2)
//tells the user that there is no warranty for the work (except to the
//extent that warranties are provided), that licensees may convey the
//work under this License, and how to view a copy of this License.  If
//the interface presents a list of user commands or options, such as a
//menu, a prominent item in the list meets this criterion.

//  1. Source Code.

//  The "source code" for a work means the preferred form of the work
//for making modifications to it.  "Object code" means any non-source
//form of a work.

//  A "Standard Interface" means an interface that either is an official
//standard defined by a recognized standards body, or, in the case of
//interfaces specified for a particular programming language, one that
//is widely used among developers working in that language.

//  The "System Libraries" of an executable work include anything, other
//than the work as a whole, that (a) is included in the normal form of
//packaging a Major Component, but which is not part of that Major
//Component, and (b) serves only to enable use of the work with that
//Major Component, or to implement a Standard Interface for which an
//implementation is available to the public in source code form.  A
//"Major Component", in this context, means a major essential component
//(kernel, window system, and so on) of the specific operating system
//(if any) on which the executable work runs, or a compiler used to
//produce the work, or an object code interpreter used to run it.

//  The "Corresponding Source" for a work in object code form means all
//the source code needed to generate, install, and (for an executable
//work) run the object code and to modify the work, including scripts to
//control those activities.  However, it does not include the work's
//System Libraries, or general-purpose tools or generally available free
//programs which are used unmodified in performing those activities but
//which are not part of the work.  For example, Corresponding Source
//includes interface definition files associated with source files for
//the work, and the source code for shared libraries and dynamically
//linked subprograms that the work is specifically designed to require,
//such as by intimate data communication or control flow between those
//subprograms and other parts of the work.

//  The Corresponding Source need not include anything that users
//can regenerate automatically from other parts of the Corresponding
//Source.

//  The Corresponding Source for a work in source code form is that
//same work.

//  2. Basic Permissions.

//  All rights granted under this License are granted for the term of
//copyright on the Program, and are irrevocable provided the stated
//conditions are met.  This License explicitly affirms your unlimited
//permission to run the unmodified Program.  The output from running a
//covered work is covered by this License only if the output, given its
//content, constitutes a covered work.  This License acknowledges your
//rights of fair use or other equivalent, as provided by copyright law.

//  You may make, run and propagate covered works that you do not
//convey, without conditions so long as your license otherwise remains
//in force.  You may convey covered works to others for the sole purpose
//of having them make modifications exclusively for you, or provide you
//with facilities for running those works, provided that you comply with
//the terms of this License in conveying all material for which you do
//not control copyright.  Those thus making or running the covered works
//for you must do so exclusively on your behalf, under your direction
//and control, on terms that prohibit them from making any copies of
//your copyrighted material outside their relationship with you.

//  Conveying under any other circumstances is permitted solely under
//the conditions stated below.  Sublicensing is not allowed; section 10
//makes it unnecessary.

//  3. Protecting Users' Legal Rights From Anti-Circumvention Law.

//  No covered work shall be deemed part of an effective technological
//measure under any applicable law fulfilling obligations under article
//11 of the WIPO copyright treaty adopted on 20 December 1996, or
//similar laws prohibiting or restricting circumvention of such
//measures.

//  When you convey a covered work, you waive any legal power to forbid
//circumvention of technological measures to the extent such circumvention
//is effected by exercising rights under this License with respect to
//the covered work, and you disclaim any intention to limit operation or
//modification of the work as a means of enforcing, against the work's
//users, your or third parties' legal rights to forbid circumvention of
//technological measures.

//  4. Conveying Verbatim Copies.

//  You may convey verbatim copies of the Program's source code as you
//receive it, in any medium, provided that you conspicuously and
//appropriately publish on each copy an appropriate copyright notice;
//keep intact all notices stating that this License and any
//non-permissive terms added in accord with section 7 apply to the code;
//keep intact all notices of the absence of any warranty; and give all
//recipients a copy of this License along with the Program.

//  You may charge any price or no price for each copy that you convey,
//and you may offer support or warranty protection for a fee.

//  5. Conveying Modified Source Versions.

//  You may convey a work based on the Program, or the modifications to
//produce it from the Program, in the form of source code under the
//terms of section 4, provided that you also meet all of these conditions:

//    a) The work must carry prominent notices stating that you modified
//    it, and giving a relevant date.

//    b) The work must carry prominent notices stating that it is
//    released under this License and any conditions added under section
//    7.  This requirement modifies the requirement in section 4 to
//    "keep intact all notices".

//    c) You must license the entire work, as a whole, under this
//    License to anyone who comes into possession of a copy.  This
//    License will therefore apply, along with any applicable section 7
//    additional terms, to the whole of the work, and all its parts,
//    regardless of how they are packaged.  This License gives no
//    permission to license the work in any other way, but it does not
//    invalidate such permission if you have separately received it.

//    d) If the work has interactive user interfaces, each must display
//    Appropriate Legal Notices; however, if the Program has interactive
//    interfaces that do not display Appropriate Legal Notices, your
//    work need not make them do so.

//  A compilation of a covered work with other separate and independent
//works, which are not by their nature extensions of the covered work,
//and which are not combined with it such as to form a larger program,
//in or on a volume of a storage or distribution medium, is called an
//"aggregate" if the compilation and its resulting copyright are not
//used to limit the access or legal rights of the compilation's users
//beyond what the individual works permit.  Inclusion of a covered work
//in an aggregate does not cause this License to apply to the other
//parts of the aggregate.

//  6. Conveying Non-Source Forms.

//  You may convey a covered work in object code form under the terms
//of sections 4 and 5, provided that you also convey the
//machine-readable Corresponding Source under the terms of this License,
//in one of these ways:

//    a) Convey the object code in, or embodied in, a physical product
//    (including a physical distribution medium), accompanied by the
//    Corresponding Source fixed on a durable physical medium
//    customarily used for software interchange.

//    b) Convey the object code in, or embodied in, a physical product
//    (including a physical distribution medium), accompanied by a
//    written offer, valid for at least three years and valid for as
//    long as you offer spare parts or customer support for that product
//    model, to give anyone who possesses the object code either (1) a
//    copy of the Corresponding Source for all the software in the
//    product that is covered by this License, on a durable physical
//    medium customarily used for software interchange, for a price no
//    more than your reasonable cost of physically performing this
//    conveying of source, or (2) access to copy the
//    Corresponding Source from a network server at no charge.

//    c) Convey individual copies of the object code with a copy of the
//    written offer to provide the Corresponding Source.  This
//    alternative is allowed only occasionally and noncommercially, and
//    only if you received the object code with such an offer, in accord
//    with subsection 6b.

//    d) Convey the object code by offering access from a designated
//    place (gratis or for a charge), and offer equivalent access to the
//    Corresponding Source in the same way through the same place at no
//    further charge.  You need not require recipients to copy the
//    Corresponding Source along with the object code.  If the place to
//    copy the object code is a network server, the Corresponding Source
//    may be on a different server (operated by you or a third party)
//    that supports equivalent copying facilities, provided you maintain
//    clear directions next to the object code saying where to find the
//    Corresponding Source.  Regardless of what server hosts the
//    Corresponding Source, you remain obligated to ensure that it is
//    available for as long as needed to satisfy these requirements.

//    e) Convey the object code using peer-to-peer transmission, provided
//    you inform other peers where the object code and Corresponding
//    Source of the work are being offered to the general public at no
//    charge under subsection 6d.

//  A separable portion of the object code, whose source code is excluded
//from the Corresponding Source as a System Library, need not be
//included in conveying the object code work.

//  A "User Product" is either (1) a "consumer product", which means any
//tangible personal property which is normally used for personal, family,
//or household purposes, or (2) anything designed or sold for incorporation
//into a dwelling.  In determining whether a product is a consumer product,
//doubtful cases shall be resolved in favor of coverage.  For a particular
//product received by a particular user, "normally used" refers to a
//typical or common use of that class of product, regardless of the status
//of the particular user or of the way in which the particular user
//actually uses, or expects or is expected to use, the product.  A product
//is a consumer product regardless of whether the product has substantial
//commercial, industrial or non-consumer uses, unless such uses represent
//the only significant mode of use of the product.

//  "Installation Information" for a User Product means any methods,
//procedures, authorization keys, or other information required to install
//and execute modified versions of a covered work in that User Product from
//a modified version of its Corresponding Source.  The information must
//suffice to ensure that the continued functioning of the modified object
//code is in no case prevented or interfered with solely because
//modification has been made.

//  If you convey an object code work under this section in, or with, or
//specifically for use in, a User Product, and the conveying occurs as
//part of a transaction in which the right of possession and use of the
//User Product is transferred to the recipient in perpetuity or for a
//fixed term (regardless of how the transaction is characterized), the
//Corresponding Source conveyed under this section must be accompanied
//by the Installation Information.  But this requirement does not apply
//if neither you nor any third party retains the ability to install
//modified object code on the User Product (for example, the work has
//been installed in ROM).

//  The requirement to provide Installation Information does not include a
//requirement to continue to provide support service, warranty, or updates
//for a work that has been modified or installed by the recipient, or for
//the User Product in which it has been modified or installed.  Access to a
//network may be denied when the modification itself materially and
//adversely affects the operation of the network or violates the rules and
//protocols for communication across the network.

//  Corresponding Source conveyed, and Installation Information provided,
//in accord with this section must be in a format that is publicly
//documented (and with an implementation available to the public in
//source code form), and must require no special password or key for
//unpacking, reading or copying.

//  7. Additional Terms.

//  "Additional permissions" are terms that supplement the terms of this
//License by making exceptions from one or more of its conditions.
//Additional permissions that are applicable to the entire Program shall
//be treated as though they were included in this License, to the extent
//that they are valid under applicable law.  If additional permissions
//apply only to part of the Program, that part may be used separately
//under those permissions, but the entire Program remains governed by
//this License without regard to the additional permissions.

//  When you convey a copy of a covered work, you may at your option
//remove any additional permissions from that copy, or from any part of
//it.  (Additional permissions may be written to require their own
//removal in certain cases when you modify the work.)  You may place
//additional permissions on material, added by you to a covered work,
//for which you have or can give appropriate copyright permission.

//  Notwithstanding any other provision of this License, for material you
//add to a covered work, you may (if authorized by the copyright holders of
//that material) supplement the terms of this License with terms:

//    a) Disclaiming warranty or limiting liability differently from the
//    terms of sections 15 and 16 of this License; or

//    b) Requiring preservation of specified reasonable legal notices or
//    author attributions in that material or in the Appropriate Legal
//    Notices displayed by works containing it; or

//    c) Prohibiting misrepresentation of the origin of that material, or
//    requiring that modified versions of such material be marked in
//    reasonable ways as different from the original version; or

//    d) Limiting the use for publicity purposes of names of licensors or
//    authors of the material; or

//    e) Declining to grant rights under trademark law for use of some
//    trade names, trademarks, or service marks; or

//    f) Requiring indemnification of licensors and authors of that
//    material by anyone who conveys the material (or modified versions of
//    it) with contractual assumptions of liability to the recipient, for
//    any liability that these contractual assumptions directly impose on
//    those licensors and authors.

//  All other non-permissive additional terms are considered "further
//restrictions" within the meaning of section 10.  If the Program as you
//received it, or any part of it, contains a notice stating that it is
//governed by this License along with a term that is a further
//restriction, you may remove that term.  If a license document contains
//a further restriction but permits relicensing or conveying under this
//License, you may add to a covered work material governed by the terms
//of that license document, provided that the further restriction does
//not survive such relicensing or conveying.

//  If you add terms to a covered work in accord with this section, you
//must place, in the relevant source files, a statement of the
//additional terms that apply to those files, or a notice indicating
//where to find the applicable terms.

//  Additional terms, permissive or non-permissive, may be stated in the
//form of a separately written license, or stated as exceptions;
//the above requirements apply either way.

//  8. Termination.

//  You may not propagate or modify a covered work except as expressly
//provided under this License.  Any attempt otherwise to propagate or
//modify it is void, and will automatically terminate your rights under
//this License (including any patent licenses granted under the third
//paragraph of section 11).

//  However, if you cease all violation of this License, then your
//license from a particular copyright holder is reinstated (a)
//provisionally, unless and until the copyright holder explicitly and
//finally terminates your license, and (b) permanently, if the copyright
//holder fails to notify you of the violation by some reasonable means
//prior to 60 days after the cessation.

//  Moreover, your license from a particular copyright holder is
//reinstated permanently if the copyright holder notifies you of the
//violation by some reasonable means, this is the first time you have
//received notice of violation of this License (for any work) from that
//copyright holder, and you cure the violation prior to 30 days after
//your receipt of the notice.

//  Termination of your rights under this section does not terminate the
//licenses of parties who have received copies or rights from you under
//this License.  If your rights have been terminated and not permanently
//reinstated, you do not qualify to receive new licenses for the same
//material under section 10.

//  9. Acceptance Not Required for Having Copies.

//  You are not required to accept this License in order to receive or
//run a copy of the Program.  Ancillary propagation of a covered work
//occurring solely as a consequence of using peer-to-peer transmission
//to receive a copy likewise does not require acceptance.  However,
//nothing other than this License grants you permission to propagate or
//modify any covered work.  These actions infringe copyright if you do
//not accept this License.  Therefore, by modifying or propagating a
//covered work, you indicate your acceptance of this License to do so.

//  10. Automatic Licensing of Downstream Recipients.

//  Each time you convey a covered work, the recipient automatically
//receives a license from the original licensors, to run, modify and
//propagate that work, subject to this License.  You are not responsible
//for enforcing compliance by third parties with this License.

//  An "entity transaction" is a transaction transferring control of an
//organization, or substantially all assets of one, or subdividing an
//organization, or merging organizations.  If propagation of a covered
//work results from an entity transaction, each party to that
//transaction who receives a copy of the work also receives whatever
//licenses to the work the party's predecessor in interest had or could
//give under the previous paragraph, plus a right to possession of the
//Corresponding Source of the work from the predecessor in interest, if
//the predecessor has it or can get it with reasonable efforts.

//  You may not impose any further restrictions on the exercise of the
//rights granted or affirmed under this License.  For example, you may
//not impose a license fee, royalty, or other charge for exercise of
//rights granted under this License, and you may not initiate litigation
//(including a cross-claim or counterclaim in a lawsuit) alleging that
//any patent claim is infringed by making, using, selling, offering for
//sale, or importing the Program or any portion of it.

//  11. Patents.

//  A "contributor" is a copyright holder who authorizes use under this
//License of the Program or a work on which the Program is based.  The
//work thus licensed is called the contributor's "contributor version".

//  A contributor's "essential patent claims" are all patent claims
//owned or controlled by the contributor, whether already acquired or
//hereafter acquired, that would be infringed by some manner, permitted
//by this License, of making, using, or selling its contributor version,
//but do not include claims that would be infringed only as a
//consequence of further modification of the contributor version.  For
//purposes of this definition, "control" includes the right to grant
//patent sublicenses in a manner consistent with the requirements of
//this License.

//  Each contributor grants you a non-exclusive, worldwide, royalty-free
//patent license under the contributor's essential patent claims, to
//make, use, sell, offer for sale, import and otherwise run, modify and
//propagate the contents of its contributor version.

//  In the following three paragraphs, a "patent license" is any express
//agreement or commitment, however denominated, not to enforce a patent
//(such as an express permission to practice a patent or covenant not to
//sue for patent infringement).  To "grant" such a patent license to a
//party means to make such an agreement or commitment not to enforce a
//patent against the party.

//  If you convey a covered work, knowingly relying on a patent license,
//and the Corresponding Source of the work is not available for anyone
//to copy, free of charge and under the terms of this License, through a
//publicly available network server or other readily accessible means,
//then you must either (1) cause the Corresponding Source to be so
//available, or (2) arrange to deprive yourself of the benefit of the
//patent license for this particular work, or (3) arrange, in a manner
//consistent with the requirements of this License, to extend the patent
//license to downstream recipients.  "Knowingly relying" means you have
//actual knowledge that, but for the patent license, your conveying the
//covered work in a country, or your recipient's use of the covered work
//in a country, would infringe one or more identifiable patents in that
//country that you have reason to believe are valid.

//  If, pursuant to or in connection with a single transaction or
//arrangement, you convey, or propagate by procuring conveyance of, a
//covered work, and grant a patent license to some of the parties
//receiving the covered work authorizing them to use, propagate, modify
//or convey a specific copy of the covered work, then the patent license
//you grant is automatically extended to all recipients of the covered
//work and works based on it.

//  A patent license is "discriminatory" if it does not include within
//the scope of its coverage, prohibits the exercise of, or is
//conditioned on the non-exercise of one or more of the rights that are
//specifically granted under this License.  You may not convey a covered
//work if you are a party to an arrangement with a third party that is
//in the business of distributing software, under which you make payment
//to the third party based on the extent of your activity of conveying
//the work, and under which the third party grants, to any of the
//parties who would receive the covered work from you, a discriminatory
//patent license (a) in connection with copies of the covered work
//conveyed by you (or copies made from those copies), or (b) primarily
//for and in connection with specific products or compilations that
//contain the covered work, unless you entered into that arrangement,
//or that patent license was granted, prior to 28 March 2007.

//  Nothing in this License shall be construed as excluding or limiting
//any implied license or other defenses to infringement that may
//otherwise be available to you under applicable patent law.

//  12. No Surrender of Others' Freedom.

//  If conditions are imposed on you (whether by court order, agreement or
//otherwise) that contradict the conditions of this License, they do not
//excuse you from the conditions of this License.  If you cannot convey a
//covered work so as to satisfy simultaneously your obligations under this
//License and any other pertinent obligations, then as a consequence you may
//not convey it at all.  For example, if you agree to terms that obligate you
//to collect a royalty for further conveying from those to whom you convey
//the Program, the only way you could satisfy both those terms and this
//License would be to refrain entirely from conveying the Program.

//  13. Use with the GNU Affero General Public License.

//  Notwithstanding any other provision of this License, you have
//permission to link or combine any covered work with a work licensed
//under version 3 of the GNU Affero General Public License into a single
//combined work, and to convey the resulting work.  The terms of this
//License will continue to apply to the part which is the covered work,
//but the special requirements of the GNU Affero General Public License,
//section 13, concerning interaction through a network will apply to the
//combination as such.

//  14. Revised Versions of this License.

//  The Free Software Foundation may publish revised and/or new versions of
//the GNU General Public License from time to time.  Such new versions will
//be similar in spirit to the present version, but may differ in detail to
//address new problems or concerns.

//  Each version is given a distinguishing version number.  If the
//Program specifies that a certain numbered version of the GNU General
//Public License "or any later version" applies to it, you have the
//option of following the terms and conditions either of that numbered
//version or of any later version published by the Free Software
//Foundation.  If the Program does not specify a version number of the
//GNU General Public License, you may choose any version ever published
//by the Free Software Foundation.

//  If the Program specifies that a proxy can decide which future
//versions of the GNU General Public License can be used, that proxy's
//public statement of acceptance of a version permanently authorizes you
//to choose that version for the Program.

//  Later license versions may give you additional or different
//permissions.  However, no additional obligations are imposed on any
//author or copyright holder as a result of your choosing to follow a
//later version.

//  15. Disclaimer of Warranty.

//  THERE IS NO WARRANTY FOR THE PROGRAM, TO THE EXTENT PERMITTED BY
//APPLICABLE LAW.  EXCEPT WHEN OTHERWISE STATED IN WRITING THE COPYRIGHT
//HOLDERS AND/OR OTHER PARTIES PROVIDE THE PROGRAM "AS IS" WITHOUT WARRANTY
//OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING, BUT NOT LIMITED TO,
//THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
//PURPOSE.  THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF THE PROGRAM
//IS WITH YOU.  SHOULD THE PROGRAM PROVE DEFECTIVE, YOU ASSUME THE COST OF
//ALL NECESSARY SERVICING, REPAIR OR CORRECTION.

//  16. Limitation of Liability.

//  IN NO EVENT UNLESS REQUIRED BY APPLICABLE LAW OR AGREED TO IN WRITING
//WILL ANY COPYRIGHT HOLDER, OR ANY OTHER PARTY WHO MODIFIES AND/OR CONVEYS
//THE PROGRAM AS PERMITTED ABOVE, BE LIABLE TO YOU FOR DAMAGES, INCLUDING ANY
//GENERAL, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES ARISING OUT OF THE
//USE OR INABILITY TO USE THE PROGRAM (INCLUDING BUT NOT LIMITED TO LOSS OF
//DATA OR DATA BEING RENDERED INACCURATE OR LOSSES SUSTAINED BY YOU OR THIRD
//PARTIES OR A FAILURE OF THE PROGRAM TO OPERATE WITH ANY OTHER PROGRAMS),
//EVEN IF SUCH HOLDER OR OTHER PARTY HAS BEEN ADVISED OF THE POSSIBILITY OF
//SUCH DAMAGES.

//  17. Interpretation of Sections 15 and 16.

//  If the disclaimer of warranty and limitation of liability provided
//above cannot be given local legal effect according to their terms,
//reviewing courts shall apply local law that most closely approximates
//an absolute waiver of all civil liability in connection with the
//Program, unless a warranty or assumption of liability accompanies a
//copy of the Program in return for a fee.

//                     END OF TERMS AND CONDITIONS

//            How to Apply These Terms to Your New Programs

//  If you develop a new program, and you want it to be of the greatest
//possible use to the public, the best way to achieve this is to make it
//free software which everyone can redistribute and change under these terms.

//  To do so, attach the following notices to the program.  It is safest
//to attach them to the start of each source file to most effectively
//state the exclusion of warranty; and each file should have at least
//the "copyright" line and a pointer to where the full notice is found.

//    <one line to give the program's name and a brief idea of what it does.>
//    Copyright (C) <year>  <name of author>

//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.

//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.

//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.

//Also add information on how to contact you by electronic and paper mail.

//  If the program does terminal interaction, make it output a short
//notice like this when it starts in an interactive mode:

//    <program>  Copyright (C) <year>  <name of author>
//    This program comes with ABSOLUTELY NO WARRANTY; for details type `show w'.
//    This is free software, and you are welcome to redistribute it
//    under certain conditions; type `show c' for details.
//    

//The hypothetical commands `show w' and `show c' should show the appropriate
//parts of the General Public License.  Of course, your program's commands
//might be different; for a GUI interface, you would use an "about box".

//  You should also get your employer (if you work as a programmer) or school,
//if any, to sign a "copyright disclaimer" for the program, if necessary.
//For more information on this, and how to apply and follow the GNU GPL, see
//<http://www.gnu.org/licenses/>.

//  The GNU General Public License does not permit incorporating your program
//into proprietary programs.  If your program is a subroutine library, you
//may consider it more useful to permit linking proprietary applications with
//the library.  If this is what you want to do, use the GNU Lesser General
//Public License instead of this License.  But first, please read
//<http://www.gnu.org/philosophy/why-not-lgpl.html>.


// 1.8d notes. tab/space/comma separated input files
// fixed error relating to jmean and min time step.
// 5960 - added baseflow instead of j_mean in get_catchment_input_points() make it similar to 1.7a
// 6001 - edited numberof points counter - was possibly overwriting by one...

// 1.8f notes - now speeds up if running with less than 9 grainsizes, vectors in outputs returns..



using System.IO;
using System;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;
using System.Text;
using System.Net;
using System.Xml;  //JMW
using System.Threading;
using System.Threading.Tasks;


namespace caesar1
{
	/// <summary>
	/// Summary description for Form1.
	/// </summary>
    /// 



	public class Form1 : System.Windows.Forms.Form
	{

       
        private System.Drawing.Bitmap m_objDrawingSurface;
		//JMW
		[System.Runtime.InteropServices.DllImport("gdi32.dll")]
		public static extern long BitBlt(IntPtr hdcDest, int nXDest, int nYDest, int nWidth, 
			int nHeight, IntPtr hdcSrc, int nXSrc, int nYSrc, int dwROP);

		private System.ComponentModel.IContainer components;
         
		//Jorge variables

        public static double magnifyValue = 0;
        public static int updateClick = 0;
        private double[] zoomFactor = { .25, .33, .50, .66, .80, 1, 1.25, 1.5, 2.0, 2.5, 3.0 };
        private double[] contrastFactor = { 1, 1.2, 1.4, 1.6, 1.8, 2.0, 2.2, 2.4, 2.6, 2.8, 3 };
        private double contrastMultiplier = 0;
        public int imageCount = 1;
        public int imageCount2 = 0;
        int coordinateDone = 0;
        double urfinalLati, urfinalLongi, llfinalLati, llfinalLongi, yurcorner, xurcorner = 0;
        public string kml = "";
        public string KML_FILE_NAME = "animation\\animation.kml";
        int save_time2, save_interval2 = 0;
        public string startDate, kmlTime;
        public DateTime googleTime;
        public string[] DateArray;
        public string[] DateArray2;
       
        
        // toms global variables
        double gravity = 9.81;
        const Single g = 9.81F;
		const Single kappa = 0.4F;
		double water_depth_erosion_threshold=0.01;
		int input_time_step=60;
		int number_of_points=0;
		double globalsediq=0;
		double time_1=1;
		double save_time=0;
		double creep_time=1;
        double creep_time2 = 1;
        double soil_erosion_time = 1;
        double soil_development_time = 1;

        double bedrock_erosion_threshold = 0;
        double bedrock_erosion_rate = 0;

		//int tot_number_of_tracer_points=0;
		int input_type_flag=0; // 0 is water input from points, 1 is input from hydrograph or rainfall file.
		double failureangle=45;
		double saveinterval=1000;
		int counter=0;
		System.Windows.Forms.Timer gameClock;
        bool googleoutputflag = false;
        double waterinput = 0;
        double waterOut = 0;
        double in_out_difference = 0;
        double mannings = 0.04;
        int rfnum = 1;

		int xmax, ymax;
        double xll, yll;

		int maxcycle;
		const int ACTIVE_FACTOR=1;
		const int TRUE=1;
		const int FALSE=0;
		double ERODEFACTOR=0.05;
		double DX=5;
		double root=7.07;

		int LIMIT=1;
		double MIN_Q=0.01;
		double CREEP_RATE=0.0025;
        double SOIL_RATE = 0.0025;
		double active=0.2;
		int G_MAX=10;
		double lateral_constant=0.0000002;
		int grain_array_tot=1;

		double time_factor = 1;
        double[] j, jo, j_mean, old_j_mean, new_j_mean, dprop, M;
		//double M=0.005;
		double baseflow=0.00000005; //end of hyd model variables usually 0.0000005 changed 2/11/05
		public static double cycle=0;
		double rain_factor=1;
		double sediQ=0;
		double grow_grass_time=0;
        double duneupdatetime = 0;
		
		double output_file_save_interval=60;
		double min_time_step=0;
		double vegTauCrit=100;
        public static int graphics_scale = 2; // value that controls the number of bmp pixels per model pixel for the output images.
        int max_time_step = 0;
        int dune_mult = 5;
        double dune_time = 1;
        double max_vel = 5;
        double sand_out = 0;
        double maxdepth = 10;
        double courant_number = 0.7;
        double erode_call = 0;
        double erode_mult = 1;
        double lateralcounter = 1;
        double edgeslope = 0.001;
        double bed_proportion = 0.01;
        double veg_lat_restriction = 0.1;
        double lateral_cross_channel_smoothing = 0.0001;
        double froude_limit = 0.8;
        double recirculate_proportion = 1;

        double Csuspmax = 0.05; // max concentration  of SS allowed in a cell (proportion)
        double hflow_threshold = 0.001;

        // KAtharine
        int variable_m_value_flag = 0;

		// grain size variables - the sizes
		double d1=0.0005;
		double d2=0.001;
		double d3=0.002;
		double d4=0.004;
		double d5=0.008;
		double d6=0.016;
		double d7=0.032;
		double d8=0.064;
		double d9=0.128;


		// Gez
		double previous;
		int hours = 0;
		double new_cycle = 0;
		double old_cycle = 0;
		double tx = 60;
		double Tx = 0;
		double tlastcalc = 0;
		double Qs_step = 0;
		double Qs_hour = 0;
		double Qs_over = 0;
		double Qs_last = 0;
		double Qw_newvol = 0;
		double Qw_oldvol = 0;
		double Qw_lastvol = 0;
		double Qw_stepvol = 0;
		double Qw_hourvol = 0;
		double Qw_hour = 0;
		double Qw_overvol = 0;
		double temptotal = 0;
		double old_sediq = 0;
        double[] sum_grain, sum_grain2, old_sum_grain,old_sum_grain2, Qg_step, Qg_step2, Qg_hour, Qg_hour2, Qg_over, Qg_over2, Qg_last,Qg_last2;
		string CATCH_FILE = "catchment.dat";
		// end gez

		// toms global arrays
        public static double[,] elev, bedrock, init_elevs, water_depth, area, tempcreep, Tau, Vel, qx, qy, qxs, qys,
            /* dune arrays */ area_depth, sand, elev2, sand2, grain, elev_diff, spat_var_mannings, erodetot, erodetot3, temp_elev;
		int[,] index,cross_scan,down_scan, rfarea;
        bool[,] inputpointsarray;
        int[] catchment_input_x_coord, catchment_input_y_coord;
		
		double[,,] vel_dir;
		double[,,] strata;
        double[]  temp_grain;
        double[,] hydrograph, dischargeinput, hourly_rain_data, hourly_m_value;
		double[,,] inputfile;
		int[,] inpoints;
		public static double[,,] veg;
		double[,] edge, edge2; //TJC 27/1/05 array for edges
        double[] old_j_mean_store;
        double[,] climate_data;
        double[, ,] sr, sl, su, sd;
        double[,] ss;

        
		// MJ global vars
		double[] fallVelocity;
		bool[] isSuspended;
		double[,] Vsusptot;
		int[] deltaX = new int[9] {0,0,1,1,1,0,-1,-1,-1};
		int[] deltaY = new int[9] {0,-1,-1,0,1,1,1,0,-1};
		int [] nActualGridCells;
		double Jw_newvol = 0.0;
		double Jw_oldvol = 0.0;
		double Jw_lastvol = 0.0;
		double Jw_stepvol = 0.0;
		double Jw_hourvol = 0.0;
		double Jw_hour = 0.0;
		double Jw_overvol = 0.0;
		double k_evap = 0.0;

		// JOE global vars
		string[] inputheader;			//Read from ASCII DEM <JOE 20050605>
		double[,] slopeAnalysis;  // Initially calculated in percent slope, coverted to radians
		double[,] aspect;		  // Radians
		double[,] hillshade;	  // 0 to 255
		double hue = 360.0;		// Ranges between 0 and 360 degrees
		double sat = 0.90;		// Ranges between 0 and 1.0 (where 1 is 100%)
		double val = 1.0;		// Ranges between 0 and 1.0 (where 1 is 100%)
		double red = 0.0;
		double green = 0.0;
		double blue = 0.0;

        // siberia submodel parameters
        double m1 = 1.70;
        double n1 = 0.69;
        double Beta3 = 0.000186;
        double m3 = 0.79;
        double Beta1 = 1067;

        // sedi tpt flags   
        int einstein = 0;
        int wilcock = 0;
        int meyer = 0;
        int div_inputs = 1;
        double rain_data_time_step = 60; // time step for rain data - default is 60. 
        double mfiletimestep = 1440; // tiem step for variable M value file


        // lisflood caesar adaptation globals
        int [] catchment_input_counter;
        int totalinputpoints = 0;

		//JMW Vars
		string basetext = "CAESAR - Lisflood 1.9j (06/08/2019)";
		string cfgname = null ;  //Config file name
		string workdir = "c:\\program files\\Caesar\\work\\";

        // stage/tidal variables
        int fromx, tox, fromy, toy;
        double stage_input_time_step = 1;
        double[] stage_inputfile;

        // Soil generation variables
        double P1, b1, k1, c1, c2, k2, c3, c4;
        

		private Graphics mygraphics;
		//private Boolean DoingGraphics;  // <JMW 20041108>
		//Form2 form2; // <JMW 20041018>
        // JMW end

        #region windows_forms_and_controls
        private System.Windows.Forms.Button button2;
		private System.Windows.Forms.MainMenu mainMenu1;
		private System.Windows.Forms.MenuItem menuItem1;
		private System.Windows.Forms.MenuItem menuItem3;
		private System.Windows.Forms.MenuItem menuItem4;
        private System.Windows.Forms.MenuItem menuItem5;
		private System.Windows.Forms.MenuItem menuItem11;
		private System.Windows.Forms.MenuItem menuItem12;
		private System.Windows.Forms.MenuItem menuItem13;
        private System.Windows.Forms.MenuItem menuItem14;
		private System.Windows.Forms.MenuItem menuItem25;
		private System.Windows.Forms.MenuItem menuItemConfigFile;
		private System.Windows.Forms.MenuItem menuItemConfigFileOpen;
		private System.Windows.Forms.MenuItem menuItemConfigFileSave;
		private System.Windows.Forms.MenuItem menuItemConfigFileSaveAs;
        private System.Windows.Forms.MenuItem menuItem2;
		private System.Windows.Forms.MenuItem menuItem9;
		private System.Windows.Forms.MenuItem menuItem8;
		private System.Windows.Forms.MenuItem menuItem7;
        private System.Windows.Forms.TabControl tabControl1;
		private System.Windows.Forms.TabPage GridTab;
		private System.Windows.Forms.TabPage NumericalTab;
		private System.Windows.Forms.TabPage GrainTab;
		private System.Windows.Forms.TabPage HydrologyTab;
		private System.Windows.Forms.TextBox gp3box;
		private System.Windows.Forms.TextBox gp4box;
		private System.Windows.Forms.TextBox gp5box;
		private System.Windows.Forms.TextBox gp6box;
		private System.Windows.Forms.TextBox gp7box;
		private System.Windows.Forms.TextBox gp8box;
		private System.Windows.Forms.TextBox gp9box;
		private System.Windows.Forms.TextBox gp2box;
		private System.Windows.Forms.TextBox gp1box;
		private System.Windows.Forms.TextBox g3box;
		private System.Windows.Forms.TextBox g4box;
		private System.Windows.Forms.TextBox g5box;
		private System.Windows.Forms.TextBox g6box;
		private System.Windows.Forms.TextBox g7box;
		private System.Windows.Forms.TextBox g8box;
		private System.Windows.Forms.TextBox g9box;
		private System.Windows.Forms.TextBox g2box;
		private System.Windows.Forms.TextBox g1box;
		private System.Windows.Forms.Label label22;
		private System.Windows.Forms.Label label21;
		private System.Windows.Forms.Label label20;
		private System.Windows.Forms.Label label19;
		private System.Windows.Forms.Label label18;
		private System.Windows.Forms.Label label17;
		private System.Windows.Forms.Label label16;
		private System.Windows.Forms.Label label15;
		private System.Windows.Forms.Label label14;
		private System.Windows.Forms.Label label13;
        private System.Windows.Forms.Label label6;
		private System.Windows.Forms.TextBox input_time_step_box;
		private System.Windows.Forms.TextBox infile4;
		private System.Windows.Forms.TextBox infile3;
		private System.Windows.Forms.TextBox infile2;
		private System.Windows.Forms.TextBox infile1;
		private System.Windows.Forms.TextBox ybox1;
		private System.Windows.Forms.TextBox ybox2;
		private System.Windows.Forms.TextBox ybox3;
		private System.Windows.Forms.TextBox ybox4;
		private System.Windows.Forms.TextBox xbox2;
		private System.Windows.Forms.TextBox xbox3;
		private System.Windows.Forms.TextBox xbox4;
		private System.Windows.Forms.TextBox xbox1;
		private System.Windows.Forms.Label label29;
		private System.Windows.Forms.Label label44;
		private System.Windows.Forms.Label label43;
		private System.Windows.Forms.Label label41;
		private System.Windows.Forms.CheckBox inbox2;
		private System.Windows.Forms.CheckBox inbox3;
		private System.Windows.Forms.CheckBox inbox4;
		private System.Windows.Forms.CheckBox inbox1;
        private System.Windows.Forms.Label label42;
		private System.Windows.Forms.TabPage DescriptionTab;
		private System.Windows.Forms.TextBox DescBox;
		private System.Windows.Forms.TextBox ytextbox;
		private System.Windows.Forms.TextBox xtextbox;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.TextBox dxbox;
        private System.Windows.Forms.Label label11;
		private System.Windows.Forms.Label label49;
		private System.Windows.Forms.TextBox mintimestepbox;
        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.TextBox smoothbox;
		private System.Windows.Forms.TextBox cyclemaxbox;
        private System.Windows.Forms.TextBox itermaxbox;
        private System.Windows.Forms.TextBox limitbox;
        private System.Windows.Forms.Label label31;
		private System.Windows.Forms.Label label27;
        private System.Windows.Forms.Label label26;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.Label label47;
		private System.Windows.Forms.StatusBar statusBar1;
		private System.Windows.Forms.StatusBarPanel IterationStatusPanel;
		private System.Windows.Forms.StatusBarPanel TimeStatusPanel;
		private System.Windows.Forms.StatusBarPanel QwStatusPanel;
		private System.Windows.Forms.StatusBarPanel QsStatusPanel;
		private System.Windows.Forms.StatusBarPanel InfoStatusPanel;
		private System.Windows.Forms.StatusBarPanel tempStatusPanel;
		private System.Windows.Forms.Button start_button;
        private System.Windows.Forms.Button button1;
		private System.Windows.Forms.MenuItem menuItem26;
        private System.Windows.Forms.MenuItem menuItem27;
		private System.Windows.Forms.MenuItem menuItem28;
		private System.Windows.Forms.MenuItem menuItem29;
		private System.Windows.Forms.CheckBox suspGS1box;
		private System.Windows.Forms.Label label4;
		private System.Windows.Forms.Label label28;
		private System.Windows.Forms.TextBox fallGS2box;
		private System.Windows.Forms.TextBox fallGS1box;
		private System.Windows.Forms.CheckBox suspGS2box;
		private System.Windows.Forms.CheckBox suspGS3box;
		private System.Windows.Forms.CheckBox suspGS4box;
		private System.Windows.Forms.CheckBox suspGS5box;
		private System.Windows.Forms.CheckBox suspGS6box;
		private System.Windows.Forms.CheckBox suspGS7box;
		private System.Windows.Forms.CheckBox suspGS8box;
		private System.Windows.Forms.CheckBox suspGS9box;
		private System.Windows.Forms.Label gpSumLabel;
		private System.Windows.Forms.Label gpSumLabel2;
		private System.Windows.Forms.TextBox fallGS3box;
		private System.Windows.Forms.TextBox fallGS4box;
		private System.Windows.Forms.TextBox fallGS5box;
		private System.Windows.Forms.TextBox fallGS6box;
		private System.Windows.Forms.TextBox fallGS7box;
		private System.Windows.Forms.TextBox fallGS8box;
		private System.Windows.Forms.TextBox fallGS9box;
		private System.Windows.Forms.MenuItem menuItem30;
        private System.Windows.Forms.MenuItem menuItem31;
		private System.Windows.Forms.MenuItem menuItem33;
		private System.Windows.Forms.MenuItem menuItem34;
        private System.Windows.Forms.CheckBox overrideheaderBox;
		private System.Windows.Forms.Button button4;
        private System.Windows.Forms.Button button3;
        private System.Windows.Forms.CheckBox recirculatebox;
		private System.Windows.Forms.Panel Panel1;
		private System.Windows.Forms.Label label52;
        private System.Windows.Forms.CheckBox bedslope_box;
        private System.Windows.Forms.ToolTip toolTip1;
		private System.Windows.Forms.TextBox tempdata1;
        private System.Windows.Forms.TextBox tempdata2;
		private System.Windows.Forms.CheckBox veltaubox;
		private System.Windows.Forms.TabPage tabPage2;
		private System.Windows.Forms.TextBox vegTauCritBox;
		private System.Windows.Forms.Label label36;
		private System.Windows.Forms.TextBox infile5;
		private System.Windows.Forms.TextBox ybox5;
		private System.Windows.Forms.TextBox xbox5;
		private System.Windows.Forms.CheckBox inbox5;
		private System.Windows.Forms.TextBox infile6;
		private System.Windows.Forms.TextBox ybox6;
		private System.Windows.Forms.TextBox xbox6;
		private System.Windows.Forms.CheckBox inbox6;
		private System.Windows.Forms.TextBox infile7;
		private System.Windows.Forms.TextBox ybox7;
		private System.Windows.Forms.TextBox xbox7;
		private System.Windows.Forms.CheckBox inbox7;
		private System.Windows.Forms.TextBox infile8;
		private System.Windows.Forms.TextBox ybox8;
		private System.Windows.Forms.TextBox xbox8;
        private System.Windows.Forms.CheckBox inbox8;
        private TabPage FilesTab;
        private CheckBox reach_mode_box;
        private CheckBox catchment_mode_box;
        private CheckBox checkBoxGenerateTimeSeries;
        private CheckBox checkBoxGenerateIterations;
        private TextBox IterationOutbox;
        private TextBox TimeseriesOutBox;
        private TextBox outputfilesaveintervalbox;
        private TextBox saveintervalbox;
        private Label label45;
        private Label label33;
        private CheckBox uniquefilecheck;
        private Label label32;
        private TextBox tracerhydrofile;
        private TextBox tracerfile;
        private TextBox bedrockbox;
        private TextBox graindataloadbox;
        private TextBox openfiletextbox;
        private CheckBox tracerbox;
        private Label label30;
        private Label label39;
        private Label label24;
        private Label label23;
        private CheckBox flowonlybox;
        private Smallwisdom.Windows.Forms.ZoomPanImageBox zoomPanImageBox1;
        private TrackBar trackBar1;
        private Label label61;
        private GroupBox groupBox2;
        private ComboBox comboBox1;
        private Label label62;
        private GroupBox groupBox3;
        private Label label63;
        private TrackBar trackBar2;
        private TextBox tracerOutputtextBox;
        private CheckBox tracerOutcheckBox;
        private TextBox mvaluebox;
        private Label label37;
        private TextBox grasstextbox;
        private Label label40;
        private TabPage tabPage4;
        private Label label67;
        private TextBox soil_ratebox;
        private TextBox slopebox;
        private TextBox creepratebox;
        private Label label34;
        private Label label8;
        private Label label69;
        private Label label68;
        private Label label70;
        private Label label73;
        private Label label72;
        private Label label71;
        private TextBox m3Box;
        private TextBox n1Box;
        private TextBox m1Box;
        private TextBox Beta3Box;
        private TextBox Beta1Box;
        private CheckBox SiberiaBox;
        private Label label75;
        private Label label74;
        private TextBox max_time_step_Box;
        private Label label76;
        private TextBox googleBeginDate;
        private Label label78;
        private TextBox googAnimationSaveInterval;
        private Label label79;
        private TextBox googleAnimationTextBox;
        private CheckBox googleAnimationCheckbox;
        private Button graphicToGoogleEarthButton;
        private CheckBox einsteinbox;
        private CheckBox wilcockbox;
        private Label label83;
        private TextBox div_inputs_box;
        private CheckBox checkBox1;
        private TabPage tabPage5;
        private CheckBox DuneBox;
        private Label label89;
        private Label label88;
        private Label label87;
        private Label label86;
        private Label label85;
        private Label label84;
        private TextBox slab_depth_box;
        private TextBox shadow_angle_box;
        private TextBox upstream_check_box;
        private TextBox depo_prob_box;
        private TextBox offset_box;
        private TextBox init_depth_box;
        private CheckBox bedslopebox2;
        private Label label56;
        private TextBox dune_time_box;
        private Label label57;
        private TextBox dune_grid_size_box;
        private GroupBox groupBox4;
        private TextBox textBox6;
        private CheckBox UTMsouthcheck;
        private TextBox UTMzonebox;
        private CheckBox UTMgridcheckbox;
        private TextBox textBox5;
        private CheckBox soilerosionBox;
        private CheckBox landslidesBox;
        private TextBox fraction_dune;
        private Label label54;
        private TextBox propremaining;
        private Label label50;
        private TextBox activebox;
        private TextBox erodefactorbox;
        private Label label12;
        private Label label48;
        private BackgroundWorker backgroundWorker1;
        private TextBox lateralratebox;
        private TextBox textBox3;
        private Label label60;
        private TextBox avge_smoothbox;
        private CheckBox newlateral;
        private Label label7;
        private TabPage tabPage1;
        private TextBox courantbox;
        private Label label38;
        private Label label53;
        private TextBox Q2box;
        private Label label3;
        private TextBox k_evapBox;
        private TextBox textBox2;
        private Label label46;
        private TextBox minqbox;
        private TextBox initscansbox;
        private Label label9;
        private Label label5;
        private CheckBox nolateral;
        private Label label55;
        private TextBox max_vel_box;
        private TextBox veg_lat_box;
        private Label label51;
        private Label label58;
        private TextBox downstreamshiftbox;
        private TextBox textBox4;
        private Label label64;
        private Label label65;
        private TextBox textBox7;
        private TextBox textBox8;
        private TextBox textBox9;
        private Label label77;
        public static CheckBox checkBox2;
        private TextBox MinQmaxvalue;
        private CheckBox checkBox3;
        private Label label90;
        private TextBox TidalFileName;
        private TextBox TidalInputStep;
        private Label label82;
        private TextBox TidalYmin;
        private TextBox TidalYmax;
        private TextBox TidalXmax;
        private TextBox TidalXmin;
        private Label label80;
        private Label label81;
        private GroupBox groupBox5;
        private GroupBox groupBox1;
        private GroupBox groupBox6;
        private Label label91;
        private TextBox textBox10;
        private TextBox bedrock_erosion_threshold_box;
        private TextBox bedrock_erosion_rate_box;
        private Label label92;
        private Label label93;
        private TabPage tabPage3;
        private Label label101;
        private Label label100;
        private Label label99;
        private TextBox textBox18;
        private TextBox textBox17;
        private TextBox textBox16;
        private Label label98;
        private Label label97;
        private Label label96;
        private TextBox textBox15;
        private TextBox textBox14;
        private TextBox textBox13;
        private Label label95;
        private Label label94;
        private TextBox textBox12;
        private TextBox textBox11;
        private CheckBox checkBox6;
        private CheckBox checkBox5;
        private CheckBox checkBox4;
        private CheckBox soildevbox;
        private GroupBox groupBox7;
        private Label label35;
        private TextBox raintimestepbox;
        private CheckBox jmeaninputfilebox;
        private Label label59;
        private TextBox mvalueloadbox;
        private TextBox raindataloadbox;
        private Label label25;
        private TextBox hydroindexBox;
        private Label label103;
        private Label label102;
        private TextBox rfnumBox;
        private CheckBox checkBox7;
        private CheckBox checkBox8;
        private TextBox textBox19;
        private Label label104;
        private CheckBox SpatVarManningsCheckbox;
        private Label label105;
        private TextBox mfiletimestepbox;
        private CheckBox meyerbox;
        private RadioButton radioButton1;
        private GroupBox groupBox8;
        private RadioButton radioButton2;
        private Label label66;
        #endregion


		public Form1()
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();

			//
			// TODO: Add any constructor code after InitializeComponent call
			//
		}

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			if( disposing )
			{
				if (components != null) 
				{
					components.Dispose();
				}
			}
			base.Dispose( disposing );
		}

		#region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
            this.components = new System.ComponentModel.Container();
            this.button2 = new System.Windows.Forms.Button();
            this.mainMenu1 = new System.Windows.Forms.MainMenu(this.components);
            this.menuItemConfigFile = new System.Windows.Forms.MenuItem();
            this.menuItemConfigFileOpen = new System.Windows.Forms.MenuItem();
            this.menuItemConfigFileSaveAs = new System.Windows.Forms.MenuItem();
            this.menuItemConfigFileSave = new System.Windows.Forms.MenuItem();
            this.menuItem1 = new System.Windows.Forms.MenuItem();
            this.menuItem30 = new System.Windows.Forms.MenuItem();
            this.menuItem3 = new System.Windows.Forms.MenuItem();
            this.menuItem4 = new System.Windows.Forms.MenuItem();
            this.menuItem5 = new System.Windows.Forms.MenuItem();
            this.menuItem2 = new System.Windows.Forms.MenuItem();
            this.menuItem7 = new System.Windows.Forms.MenuItem();
            this.menuItem8 = new System.Windows.Forms.MenuItem();
            this.menuItem9 = new System.Windows.Forms.MenuItem();
            this.menuItem26 = new System.Windows.Forms.MenuItem();
            this.menuItem27 = new System.Windows.Forms.MenuItem();
            this.menuItem28 = new System.Windows.Forms.MenuItem();
            this.menuItem31 = new System.Windows.Forms.MenuItem();
            this.menuItem11 = new System.Windows.Forms.MenuItem();
            this.menuItem12 = new System.Windows.Forms.MenuItem();
            this.menuItem13 = new System.Windows.Forms.MenuItem();
            this.menuItem14 = new System.Windows.Forms.MenuItem();
            this.menuItem25 = new System.Windows.Forms.MenuItem();
            this.menuItem29 = new System.Windows.Forms.MenuItem();
            this.menuItem33 = new System.Windows.Forms.MenuItem();
            this.menuItem34 = new System.Windows.Forms.MenuItem();
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.FilesTab = new System.Windows.Forms.TabPage();
            this.groupBox6 = new System.Windows.Forms.GroupBox();
            this.groupBox4 = new System.Windows.Forms.GroupBox();
            this.textBox6 = new System.Windows.Forms.TextBox();
            this.UTMsouthcheck = new System.Windows.Forms.CheckBox();
            this.UTMzonebox = new System.Windows.Forms.TextBox();
            this.UTMgridcheckbox = new System.Windows.Forms.CheckBox();
            this.textBox5 = new System.Windows.Forms.TextBox();
            this.googleBeginDate = new System.Windows.Forms.TextBox();
            this.label78 = new System.Windows.Forms.Label();
            this.googAnimationSaveInterval = new System.Windows.Forms.TextBox();
            this.label79 = new System.Windows.Forms.Label();
            this.googleAnimationTextBox = new System.Windows.Forms.TextBox();
            this.googleAnimationCheckbox = new System.Windows.Forms.CheckBox();
            this.checkBox3 = new System.Windows.Forms.CheckBox();
            this.outputfilesaveintervalbox = new System.Windows.Forms.TextBox();
            this.TimeseriesOutBox = new System.Windows.Forms.TextBox();
            this.tracerOutputtextBox = new System.Windows.Forms.TextBox();
            this.tracerOutcheckBox = new System.Windows.Forms.CheckBox();
            this.reach_mode_box = new System.Windows.Forms.CheckBox();
            this.catchment_mode_box = new System.Windows.Forms.CheckBox();
            this.checkBoxGenerateTimeSeries = new System.Windows.Forms.CheckBox();
            this.checkBoxGenerateIterations = new System.Windows.Forms.CheckBox();
            this.IterationOutbox = new System.Windows.Forms.TextBox();
            this.saveintervalbox = new System.Windows.Forms.TextBox();
            this.label45 = new System.Windows.Forms.Label();
            this.label33 = new System.Windows.Forms.Label();
            this.uniquefilecheck = new System.Windows.Forms.CheckBox();
            this.label32 = new System.Windows.Forms.Label();
            this.tracerhydrofile = new System.Windows.Forms.TextBox();
            this.tracerfile = new System.Windows.Forms.TextBox();
            this.bedrockbox = new System.Windows.Forms.TextBox();
            this.graindataloadbox = new System.Windows.Forms.TextBox();
            this.openfiletextbox = new System.Windows.Forms.TextBox();
            this.tracerbox = new System.Windows.Forms.CheckBox();
            this.label30 = new System.Windows.Forms.Label();
            this.label39 = new System.Windows.Forms.Label();
            this.label24 = new System.Windows.Forms.Label();
            this.label23 = new System.Windows.Forms.Label();
            this.NumericalTab = new System.Windows.Forms.TabPage();
            this.bedslopebox2 = new System.Windows.Forms.CheckBox();
            this.max_time_step_Box = new System.Windows.Forms.TextBox();
            this.label76 = new System.Windows.Forms.Label();
            this.veltaubox = new System.Windows.Forms.CheckBox();
            this.label52 = new System.Windows.Forms.Label();
            this.bedslope_box = new System.Windows.Forms.CheckBox();
            this.label47 = new System.Windows.Forms.Label();
            this.mintimestepbox = new System.Windows.Forms.TextBox();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.smoothbox = new System.Windows.Forms.TextBox();
            this.cyclemaxbox = new System.Windows.Forms.TextBox();
            this.itermaxbox = new System.Windows.Forms.TextBox();
            this.limitbox = new System.Windows.Forms.TextBox();
            this.label31 = new System.Windows.Forms.Label();
            this.label27 = new System.Windows.Forms.Label();
            this.label26 = new System.Windows.Forms.Label();
            this.label10 = new System.Windows.Forms.Label();
            this.label49 = new System.Windows.Forms.Label();
            this.GrainTab = new System.Windows.Forms.TabPage();
            this.meyerbox = new System.Windows.Forms.CheckBox();
            this.checkBox8 = new System.Windows.Forms.CheckBox();
            this.bedrock_erosion_threshold_box = new System.Windows.Forms.TextBox();
            this.bedrock_erosion_rate_box = new System.Windows.Forms.TextBox();
            this.label92 = new System.Windows.Forms.Label();
            this.label93 = new System.Windows.Forms.Label();
            this.label65 = new System.Windows.Forms.Label();
            this.textBox7 = new System.Windows.Forms.TextBox();
            this.label58 = new System.Windows.Forms.Label();
            this.downstreamshiftbox = new System.Windows.Forms.TextBox();
            this.label55 = new System.Windows.Forms.Label();
            this.max_vel_box = new System.Windows.Forms.TextBox();
            this.textBox3 = new System.Windows.Forms.TextBox();
            this.label60 = new System.Windows.Forms.Label();
            this.avge_smoothbox = new System.Windows.Forms.TextBox();
            this.nolateral = new System.Windows.Forms.CheckBox();
            this.newlateral = new System.Windows.Forms.CheckBox();
            this.label7 = new System.Windows.Forms.Label();
            this.lateralratebox = new System.Windows.Forms.TextBox();
            this.label48 = new System.Windows.Forms.Label();
            this.label54 = new System.Windows.Forms.Label();
            this.propremaining = new System.Windows.Forms.TextBox();
            this.label50 = new System.Windows.Forms.Label();
            this.activebox = new System.Windows.Forms.TextBox();
            this.erodefactorbox = new System.Windows.Forms.TextBox();
            this.label12 = new System.Windows.Forms.Label();
            this.einsteinbox = new System.Windows.Forms.CheckBox();
            this.wilcockbox = new System.Windows.Forms.CheckBox();
            this.fallGS9box = new System.Windows.Forms.TextBox();
            this.fallGS8box = new System.Windows.Forms.TextBox();
            this.fallGS7box = new System.Windows.Forms.TextBox();
            this.fallGS6box = new System.Windows.Forms.TextBox();
            this.fallGS5box = new System.Windows.Forms.TextBox();
            this.fallGS4box = new System.Windows.Forms.TextBox();
            this.fallGS3box = new System.Windows.Forms.TextBox();
            this.gpSumLabel = new System.Windows.Forms.Label();
            this.gpSumLabel2 = new System.Windows.Forms.Label();
            this.suspGS9box = new System.Windows.Forms.CheckBox();
            this.suspGS8box = new System.Windows.Forms.CheckBox();
            this.suspGS7box = new System.Windows.Forms.CheckBox();
            this.suspGS6box = new System.Windows.Forms.CheckBox();
            this.suspGS5box = new System.Windows.Forms.CheckBox();
            this.suspGS4box = new System.Windows.Forms.CheckBox();
            this.suspGS3box = new System.Windows.Forms.CheckBox();
            this.suspGS2box = new System.Windows.Forms.CheckBox();
            this.fallGS2box = new System.Windows.Forms.TextBox();
            this.fallGS1box = new System.Windows.Forms.TextBox();
            this.label28 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.suspGS1box = new System.Windows.Forms.CheckBox();
            this.gp3box = new System.Windows.Forms.TextBox();
            this.gp4box = new System.Windows.Forms.TextBox();
            this.gp5box = new System.Windows.Forms.TextBox();
            this.gp6box = new System.Windows.Forms.TextBox();
            this.gp7box = new System.Windows.Forms.TextBox();
            this.gp8box = new System.Windows.Forms.TextBox();
            this.gp9box = new System.Windows.Forms.TextBox();
            this.gp2box = new System.Windows.Forms.TextBox();
            this.gp1box = new System.Windows.Forms.TextBox();
            this.g3box = new System.Windows.Forms.TextBox();
            this.g4box = new System.Windows.Forms.TextBox();
            this.g5box = new System.Windows.Forms.TextBox();
            this.g6box = new System.Windows.Forms.TextBox();
            this.g7box = new System.Windows.Forms.TextBox();
            this.g8box = new System.Windows.Forms.TextBox();
            this.g9box = new System.Windows.Forms.TextBox();
            this.g2box = new System.Windows.Forms.TextBox();
            this.g1box = new System.Windows.Forms.TextBox();
            this.label22 = new System.Windows.Forms.Label();
            this.label21 = new System.Windows.Forms.Label();
            this.label20 = new System.Windows.Forms.Label();
            this.label19 = new System.Windows.Forms.Label();
            this.label18 = new System.Windows.Forms.Label();
            this.label17 = new System.Windows.Forms.Label();
            this.label16 = new System.Windows.Forms.Label();
            this.label15 = new System.Windows.Forms.Label();
            this.label14 = new System.Windows.Forms.Label();
            this.label13 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.DescriptionTab = new System.Windows.Forms.TabPage();
            this.DescBox = new System.Windows.Forms.TextBox();
            this.GridTab = new System.Windows.Forms.TabPage();
            this.overrideheaderBox = new System.Windows.Forms.CheckBox();
            this.dxbox = new System.Windows.Forms.TextBox();
            this.label11 = new System.Windows.Forms.Label();
            this.ytextbox = new System.Windows.Forms.TextBox();
            this.xtextbox = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.HydrologyTab = new System.Windows.Forms.TabPage();
            this.groupBox7 = new System.Windows.Forms.GroupBox();
            this.label105 = new System.Windows.Forms.Label();
            this.mfiletimestepbox = new System.Windows.Forms.TextBox();
            this.hydroindexBox = new System.Windows.Forms.TextBox();
            this.label103 = new System.Windows.Forms.Label();
            this.label37 = new System.Windows.Forms.Label();
            this.mvaluebox = new System.Windows.Forms.TextBox();
            this.label102 = new System.Windows.Forms.Label();
            this.rfnumBox = new System.Windows.Forms.TextBox();
            this.checkBox7 = new System.Windows.Forms.CheckBox();
            this.label35 = new System.Windows.Forms.Label();
            this.raintimestepbox = new System.Windows.Forms.TextBox();
            this.jmeaninputfilebox = new System.Windows.Forms.CheckBox();
            this.label59 = new System.Windows.Forms.Label();
            this.mvalueloadbox = new System.Windows.Forms.TextBox();
            this.raindataloadbox = new System.Windows.Forms.TextBox();
            this.label25 = new System.Windows.Forms.Label();
            this.groupBox5 = new System.Windows.Forms.GroupBox();
            this.label83 = new System.Windows.Forms.Label();
            this.div_inputs_box = new System.Windows.Forms.TextBox();
            this.infile8 = new System.Windows.Forms.TextBox();
            this.ybox8 = new System.Windows.Forms.TextBox();
            this.xbox8 = new System.Windows.Forms.TextBox();
            this.inbox8 = new System.Windows.Forms.CheckBox();
            this.infile7 = new System.Windows.Forms.TextBox();
            this.ybox7 = new System.Windows.Forms.TextBox();
            this.xbox7 = new System.Windows.Forms.TextBox();
            this.inbox7 = new System.Windows.Forms.CheckBox();
            this.infile6 = new System.Windows.Forms.TextBox();
            this.ybox6 = new System.Windows.Forms.TextBox();
            this.xbox6 = new System.Windows.Forms.TextBox();
            this.inbox6 = new System.Windows.Forms.CheckBox();
            this.infile5 = new System.Windows.Forms.TextBox();
            this.ybox5 = new System.Windows.Forms.TextBox();
            this.xbox5 = new System.Windows.Forms.TextBox();
            this.inbox5 = new System.Windows.Forms.CheckBox();
            this.input_time_step_box = new System.Windows.Forms.TextBox();
            this.infile4 = new System.Windows.Forms.TextBox();
            this.infile3 = new System.Windows.Forms.TextBox();
            this.infile2 = new System.Windows.Forms.TextBox();
            this.infile1 = new System.Windows.Forms.TextBox();
            this.ybox1 = new System.Windows.Forms.TextBox();
            this.ybox2 = new System.Windows.Forms.TextBox();
            this.ybox3 = new System.Windows.Forms.TextBox();
            this.ybox4 = new System.Windows.Forms.TextBox();
            this.xbox2 = new System.Windows.Forms.TextBox();
            this.xbox3 = new System.Windows.Forms.TextBox();
            this.xbox4 = new System.Windows.Forms.TextBox();
            this.xbox1 = new System.Windows.Forms.TextBox();
            this.label29 = new System.Windows.Forms.Label();
            this.label44 = new System.Windows.Forms.Label();
            this.label43 = new System.Windows.Forms.Label();
            this.label41 = new System.Windows.Forms.Label();
            this.inbox2 = new System.Windows.Forms.CheckBox();
            this.inbox3 = new System.Windows.Forms.CheckBox();
            this.inbox4 = new System.Windows.Forms.CheckBox();
            this.inbox1 = new System.Windows.Forms.CheckBox();
            this.label42 = new System.Windows.Forms.Label();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.label90 = new System.Windows.Forms.Label();
            this.TidalFileName = new System.Windows.Forms.TextBox();
            this.TidalInputStep = new System.Windows.Forms.TextBox();
            this.label82 = new System.Windows.Forms.Label();
            this.TidalYmin = new System.Windows.Forms.TextBox();
            this.TidalYmax = new System.Windows.Forms.TextBox();
            this.TidalXmax = new System.Windows.Forms.TextBox();
            this.TidalXmin = new System.Windows.Forms.TextBox();
            this.label80 = new System.Windows.Forms.Label();
            this.label81 = new System.Windows.Forms.Label();
            this.tabPage2 = new System.Windows.Forms.TabPage();
            this.groupBox8 = new System.Windows.Forms.GroupBox();
            this.radioButton2 = new System.Windows.Forms.RadioButton();
            this.radioButton1 = new System.Windows.Forms.RadioButton();
            this.veg_lat_box = new System.Windows.Forms.TextBox();
            this.label51 = new System.Windows.Forms.Label();
            this.grasstextbox = new System.Windows.Forms.TextBox();
            this.label40 = new System.Windows.Forms.Label();
            this.label36 = new System.Windows.Forms.Label();
            this.vegTauCritBox = new System.Windows.Forms.TextBox();
            this.tabPage4 = new System.Windows.Forms.TabPage();
            this.soilerosionBox = new System.Windows.Forms.CheckBox();
            this.landslidesBox = new System.Windows.Forms.CheckBox();
            this.label75 = new System.Windows.Forms.Label();
            this.label74 = new System.Windows.Forms.Label();
            this.label73 = new System.Windows.Forms.Label();
            this.label72 = new System.Windows.Forms.Label();
            this.label71 = new System.Windows.Forms.Label();
            this.m3Box = new System.Windows.Forms.TextBox();
            this.n1Box = new System.Windows.Forms.TextBox();
            this.m1Box = new System.Windows.Forms.TextBox();
            this.Beta3Box = new System.Windows.Forms.TextBox();
            this.Beta1Box = new System.Windows.Forms.TextBox();
            this.SiberiaBox = new System.Windows.Forms.CheckBox();
            this.label70 = new System.Windows.Forms.Label();
            this.label69 = new System.Windows.Forms.Label();
            this.label68 = new System.Windows.Forms.Label();
            this.label67 = new System.Windows.Forms.Label();
            this.soil_ratebox = new System.Windows.Forms.TextBox();
            this.slopebox = new System.Windows.Forms.TextBox();
            this.creepratebox = new System.Windows.Forms.TextBox();
            this.label34 = new System.Windows.Forms.Label();
            this.label8 = new System.Windows.Forms.Label();
            this.tabPage5 = new System.Windows.Forms.TabPage();
            this.label91 = new System.Windows.Forms.Label();
            this.textBox10 = new System.Windows.Forms.TextBox();
            this.fraction_dune = new System.Windows.Forms.TextBox();
            this.label57 = new System.Windows.Forms.Label();
            this.dune_grid_size_box = new System.Windows.Forms.TextBox();
            this.label56 = new System.Windows.Forms.Label();
            this.dune_time_box = new System.Windows.Forms.TextBox();
            this.label89 = new System.Windows.Forms.Label();
            this.label88 = new System.Windows.Forms.Label();
            this.label87 = new System.Windows.Forms.Label();
            this.label86 = new System.Windows.Forms.Label();
            this.label85 = new System.Windows.Forms.Label();
            this.label84 = new System.Windows.Forms.Label();
            this.slab_depth_box = new System.Windows.Forms.TextBox();
            this.shadow_angle_box = new System.Windows.Forms.TextBox();
            this.upstream_check_box = new System.Windows.Forms.TextBox();
            this.depo_prob_box = new System.Windows.Forms.TextBox();
            this.offset_box = new System.Windows.Forms.TextBox();
            this.init_depth_box = new System.Windows.Forms.TextBox();
            this.DuneBox = new System.Windows.Forms.CheckBox();
            this.tabPage1 = new System.Windows.Forms.TabPage();
            this.textBox19 = new System.Windows.Forms.TextBox();
            this.label104 = new System.Windows.Forms.Label();
            this.SpatVarManningsCheckbox = new System.Windows.Forms.CheckBox();
            this.MinQmaxvalue = new System.Windows.Forms.TextBox();
            this.textBox9 = new System.Windows.Forms.TextBox();
            this.label77 = new System.Windows.Forms.Label();
            this.textBox8 = new System.Windows.Forms.TextBox();
            this.label66 = new System.Windows.Forms.Label();
            this.textBox4 = new System.Windows.Forms.TextBox();
            this.label64 = new System.Windows.Forms.Label();
            this.courantbox = new System.Windows.Forms.TextBox();
            this.label38 = new System.Windows.Forms.Label();
            this.label53 = new System.Windows.Forms.Label();
            this.Q2box = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.k_evapBox = new System.Windows.Forms.TextBox();
            this.textBox2 = new System.Windows.Forms.TextBox();
            this.label46 = new System.Windows.Forms.Label();
            this.minqbox = new System.Windows.Forms.TextBox();
            this.initscansbox = new System.Windows.Forms.TextBox();
            this.label9 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.tabPage3 = new System.Windows.Forms.TabPage();
            this.label101 = new System.Windows.Forms.Label();
            this.label100 = new System.Windows.Forms.Label();
            this.label99 = new System.Windows.Forms.Label();
            this.textBox18 = new System.Windows.Forms.TextBox();
            this.textBox17 = new System.Windows.Forms.TextBox();
            this.textBox16 = new System.Windows.Forms.TextBox();
            this.label98 = new System.Windows.Forms.Label();
            this.label97 = new System.Windows.Forms.Label();
            this.label96 = new System.Windows.Forms.Label();
            this.textBox15 = new System.Windows.Forms.TextBox();
            this.textBox14 = new System.Windows.Forms.TextBox();
            this.textBox13 = new System.Windows.Forms.TextBox();
            this.label95 = new System.Windows.Forms.Label();
            this.label94 = new System.Windows.Forms.Label();
            this.textBox12 = new System.Windows.Forms.TextBox();
            this.textBox11 = new System.Windows.Forms.TextBox();
            this.checkBox6 = new System.Windows.Forms.CheckBox();
            this.checkBox5 = new System.Windows.Forms.CheckBox();
            this.checkBox4 = new System.Windows.Forms.CheckBox();
            this.soildevbox = new System.Windows.Forms.CheckBox();
            this.Panel1 = new System.Windows.Forms.Panel();
            this.tempdata2 = new System.Windows.Forms.TextBox();
            this.tempdata1 = new System.Windows.Forms.TextBox();
            this.graphicToGoogleEarthButton = new System.Windows.Forms.Button();
            this.button4 = new System.Windows.Forms.Button();
            this.button3 = new System.Windows.Forms.Button();
            this.recirculatebox = new System.Windows.Forms.CheckBox();
            this.flowonlybox = new System.Windows.Forms.CheckBox();
            this.statusBar1 = new System.Windows.Forms.StatusBar();
            this.InfoStatusPanel = new System.Windows.Forms.StatusBarPanel();
            this.IterationStatusPanel = new System.Windows.Forms.StatusBarPanel();
            this.TimeStatusPanel = new System.Windows.Forms.StatusBarPanel();
            this.QwStatusPanel = new System.Windows.Forms.StatusBarPanel();
            this.QsStatusPanel = new System.Windows.Forms.StatusBarPanel();
            this.tempStatusPanel = new System.Windows.Forms.StatusBarPanel();
            this.start_button = new System.Windows.Forms.Button();
            this.button1 = new System.Windows.Forms.Button();
            checkBox2 = new System.Windows.Forms.CheckBox();
            this.checkBox1 = new System.Windows.Forms.CheckBox();
            this.toolTip1 = new System.Windows.Forms.ToolTip(this.components);
            this.trackBar1 = new System.Windows.Forms.TrackBar();
            this.label61 = new System.Windows.Forms.Label();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.label62 = new System.Windows.Forms.Label();
            this.comboBox1 = new System.Windows.Forms.ComboBox();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.label63 = new System.Windows.Forms.Label();
            this.trackBar2 = new System.Windows.Forms.TrackBar();
            this.backgroundWorker1 = new System.ComponentModel.BackgroundWorker();
            this.zoomPanImageBox1 = new Smallwisdom.Windows.Forms.ZoomPanImageBox();
            this.tabControl1.SuspendLayout();
            this.FilesTab.SuspendLayout();
            this.groupBox6.SuspendLayout();
            this.groupBox4.SuspendLayout();
            this.NumericalTab.SuspendLayout();
            this.GrainTab.SuspendLayout();
            this.DescriptionTab.SuspendLayout();
            this.GridTab.SuspendLayout();
            this.HydrologyTab.SuspendLayout();
            this.groupBox7.SuspendLayout();
            this.groupBox5.SuspendLayout();
            this.groupBox1.SuspendLayout();
            this.tabPage2.SuspendLayout();
            this.groupBox8.SuspendLayout();
            this.tabPage4.SuspendLayout();
            this.tabPage5.SuspendLayout();
            this.tabPage1.SuspendLayout();
            this.tabPage3.SuspendLayout();
            this.Panel1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.InfoStatusPanel)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.IterationStatusPanel)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.TimeStatusPanel)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.QwStatusPanel)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.QsStatusPanel)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.tempStatusPanel)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBar1)).BeginInit();
            this.groupBox2.SuspendLayout();
            this.groupBox3.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.trackBar2)).BeginInit();
            this.SuspendLayout();
            // 
            // button2
            // 
            this.button2.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.button2.Location = new System.Drawing.Point(9, 513);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(100, 25);
            this.button2.TabIndex = 7;
            this.button2.Text = "load data";
            this.button2.Click += new System.EventHandler(this.button2_Click);
            // 
            // mainMenu1
            // 
            this.mainMenu1.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
            this.menuItemConfigFile,
            this.menuItem1,
            this.menuItem2,
            this.menuItem11});
            // 
            // menuItemConfigFile
            // 
            this.menuItemConfigFile.Index = 0;
            this.menuItemConfigFile.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
            this.menuItemConfigFileOpen,
            this.menuItemConfigFileSaveAs,
            this.menuItemConfigFileSave});
            this.menuItemConfigFile.Text = "Config &File";
            // 
            // menuItemConfigFileOpen
            // 
            this.menuItemConfigFileOpen.Index = 0;
            this.menuItemConfigFileOpen.Text = "&Open";
            this.menuItemConfigFileOpen.Click += new System.EventHandler(this.menuItemConfigFileOpen_Click);
            // 
            // menuItemConfigFileSaveAs
            // 
            this.menuItemConfigFileSaveAs.Index = 1;
            this.menuItemConfigFileSaveAs.Text = "Save &As";
            this.menuItemConfigFileSaveAs.Click += new System.EventHandler(this.menuItemConfigFileSave_Click);
            // 
            // menuItemConfigFileSave
            // 
            this.menuItemConfigFileSave.Index = 2;
            this.menuItemConfigFileSave.Text = "&Save";
            this.menuItemConfigFileSave.Click += new System.EventHandler(this.menuItemConfigFileSave_Click);
            // 
            // menuItem1
            // 
            this.menuItem1.Index = 1;
            this.menuItem1.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
            this.menuItem30,
            this.menuItem3,
            this.menuItem4,
            this.menuItem5});
            this.menuItem1.Text = "&Top Graphics";
            // 
            // menuItem30
            // 
            this.menuItem30.Index = 0;
            this.menuItem30.Text = "DEM";
            this.menuItem30.Click += new System.EventHandler(this.menuItem30_Click);
            // 
            // menuItem3
            // 
            this.menuItem3.Checked = true;
            this.menuItem3.Index = 1;
            this.menuItem3.Text = "water depth (low scale)";
            this.menuItem3.Click += new System.EventHandler(this.menuItem3_Click);
            // 
            // menuItem4
            // 
            this.menuItem4.Index = 2;
            this.menuItem4.Text = "erosion/dep";
            this.menuItem4.Click += new System.EventHandler(this.menuItem4_Click);
            // 
            // menuItem5
            // 
            this.menuItem5.Index = 3;
            this.menuItem5.Text = "Grass/veg cover";
            this.menuItem5.Click += new System.EventHandler(this.menuItem5_Click);
            // 
            // menuItem2
            // 
            this.menuItem2.Index = 2;
            this.menuItem2.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
            this.menuItem7,
            this.menuItem8,
            this.menuItem9,
            this.menuItem26,
            this.menuItem27,
            this.menuItem28,
            this.menuItem31});
            this.menuItem2.Text = "&Top graphics II";
            // 
            // menuItem7
            // 
            this.menuItem7.Index = 0;
            this.menuItem7.Text = "lateral gradient";
            this.menuItem7.Click += new System.EventHandler(this.menuItem7_Click);
            // 
            // menuItem8
            // 
            this.menuItem8.Index = 1;
            this.menuItem8.Text = "Bed sheer stress";
            this.menuItem8.Click += new System.EventHandler(this.menuItem8_Click);
            // 
            // menuItem9
            // 
            this.menuItem9.Index = 2;
            this.menuItem9.Text = "grainsize (new scale)";
            this.menuItem9.Click += new System.EventHandler(this.menuItem9_Click);
            // 
            // menuItem26
            // 
            this.menuItem26.Index = 3;
            this.menuItem26.Text = "Drainage area";
            this.menuItem26.Click += new System.EventHandler(this.menuItem26_Click);
            // 
            // menuItem27
            // 
            this.menuItem27.Index = 4;
            this.menuItem27.Text = "susp conc";
            this.menuItem27.Click += new System.EventHandler(this.menuItem27_Click);
            // 
            // menuItem28
            // 
            this.menuItem28.Index = 5;
            this.menuItem28.Text = "soil depth";
            this.menuItem28.Click += new System.EventHandler(this.menuItem28_Click);
            // 
            // menuItem31
            // 
            this.menuItem31.Index = 6;
            this.menuItem31.Text = "flow velocity";
            this.menuItem31.Click += new System.EventHandler(this.menuItem31_Click);
            // 
            // menuItem11
            // 
            this.menuItem11.Index = 3;
            this.menuItem11.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
            this.menuItem12,
            this.menuItem13,
            this.menuItem14,
            this.menuItem25,
            this.menuItem29,
            this.menuItem33,
            this.menuItem34});
            this.menuItem11.Text = "Save Options";
            // 
            // menuItem12
            // 
            this.menuItem12.Checked = true;
            this.menuItem12.Index = 0;
            this.menuItem12.Text = "elevations";
            this.menuItem12.Click += new System.EventHandler(this.menuItem12_Click);
            // 
            // menuItem13
            // 
            this.menuItem13.Checked = true;
            this.menuItem13.Index = 1;
            this.menuItem13.Text = "elev diff";
            this.menuItem13.Click += new System.EventHandler(this.menuItem13_Click);
            // 
            // menuItem14
            // 
            this.menuItem14.Checked = true;
            this.menuItem14.Index = 2;
            this.menuItem14.Text = "grainsize";
            this.menuItem14.Click += new System.EventHandler(this.menuItem14_Click);
            // 
            // menuItem25
            // 
            this.menuItem25.Index = 3;
            this.menuItem25.Text = "water depth";
            this.menuItem25.Click += new System.EventHandler(this.menuItem25_Click);
            // 
            // menuItem29
            // 
            this.menuItem29.Checked = true;
            this.menuItem29.Index = 4;
            this.menuItem29.Text = "d50 top layer";
            this.menuItem29.Click += new System.EventHandler(this.menuItem29_Click);
            // 
            // menuItem33
            // 
            this.menuItem33.Index = 5;
            this.menuItem33.Text = "flow velocity";
            this.menuItem33.Click += new System.EventHandler(this.menuItem33_Click);
            // 
            // menuItem34
            // 
            this.menuItem34.Index = 6;
            this.menuItem34.Text = "Veloc vectors";
            this.menuItem34.Click += new System.EventHandler(this.menuItem34_Click);
            // 
            // tabControl1
            // 
            this.tabControl1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.tabControl1.Controls.Add(this.FilesTab);
            this.tabControl1.Controls.Add(this.NumericalTab);
            this.tabControl1.Controls.Add(this.GrainTab);
            this.tabControl1.Controls.Add(this.DescriptionTab);
            this.tabControl1.Controls.Add(this.GridTab);
            this.tabControl1.Controls.Add(this.HydrologyTab);
            this.tabControl1.Controls.Add(this.tabPage2);
            this.tabControl1.Controls.Add(this.tabPage4);
            this.tabControl1.Controls.Add(this.tabPage5);
            this.tabControl1.Controls.Add(this.tabPage1);
            this.tabControl1.Controls.Add(this.tabPage3);
            this.tabControl1.Location = new System.Drawing.Point(3, -1);
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(1331, 513);
            this.tabControl1.TabIndex = 143;
            this.tabControl1.Tag = "Flow Model";
            // 
            // FilesTab
            // 
            this.FilesTab.Controls.Add(this.groupBox6);
            this.FilesTab.Controls.Add(this.checkBox3);
            this.FilesTab.Controls.Add(this.outputfilesaveintervalbox);
            this.FilesTab.Controls.Add(this.TimeseriesOutBox);
            this.FilesTab.Controls.Add(this.tracerOutputtextBox);
            this.FilesTab.Controls.Add(this.tracerOutcheckBox);
            this.FilesTab.Controls.Add(this.reach_mode_box);
            this.FilesTab.Controls.Add(this.catchment_mode_box);
            this.FilesTab.Controls.Add(this.checkBoxGenerateTimeSeries);
            this.FilesTab.Controls.Add(this.checkBoxGenerateIterations);
            this.FilesTab.Controls.Add(this.IterationOutbox);
            this.FilesTab.Controls.Add(this.saveintervalbox);
            this.FilesTab.Controls.Add(this.label45);
            this.FilesTab.Controls.Add(this.label33);
            this.FilesTab.Controls.Add(this.uniquefilecheck);
            this.FilesTab.Controls.Add(this.label32);
            this.FilesTab.Controls.Add(this.tracerhydrofile);
            this.FilesTab.Controls.Add(this.tracerfile);
            this.FilesTab.Controls.Add(this.bedrockbox);
            this.FilesTab.Controls.Add(this.graindataloadbox);
            this.FilesTab.Controls.Add(this.openfiletextbox);
            this.FilesTab.Controls.Add(this.tracerbox);
            this.FilesTab.Controls.Add(this.label30);
            this.FilesTab.Controls.Add(this.label39);
            this.FilesTab.Controls.Add(this.label24);
            this.FilesTab.Controls.Add(this.label23);
            this.FilesTab.Location = new System.Drawing.Point(4, 22);
            this.FilesTab.Name = "FilesTab";
            this.FilesTab.Size = new System.Drawing.Size(1323, 487);
            this.FilesTab.TabIndex = 0;
            this.FilesTab.Text = "Files";
            this.FilesTab.UseVisualStyleBackColor = true;
            // 
            // groupBox6
            // 
            this.groupBox6.Controls.Add(this.groupBox4);
            this.groupBox6.Controls.Add(this.UTMgridcheckbox);
            this.groupBox6.Controls.Add(this.textBox5);
            this.groupBox6.Controls.Add(this.googleBeginDate);
            this.groupBox6.Controls.Add(this.label78);
            this.groupBox6.Controls.Add(this.googAnimationSaveInterval);
            this.groupBox6.Controls.Add(this.label79);
            this.groupBox6.Controls.Add(this.googleAnimationTextBox);
            this.groupBox6.Controls.Add(this.googleAnimationCheckbox);
            this.groupBox6.Location = new System.Drawing.Point(367, 192);
            this.groupBox6.Name = "groupBox6";
            this.groupBox6.Size = new System.Drawing.Size(418, 236);
            this.groupBox6.TabIndex = 205;
            this.groupBox6.TabStop = false;
            this.groupBox6.Text = "Google Earth output variables";
            // 
            // groupBox4
            // 
            this.groupBox4.Controls.Add(this.textBox6);
            this.groupBox4.Controls.Add(this.UTMsouthcheck);
            this.groupBox4.Controls.Add(this.UTMzonebox);
            this.groupBox4.Location = new System.Drawing.Point(220, 137);
            this.groupBox4.Name = "groupBox4";
            this.groupBox4.Size = new System.Drawing.Size(144, 65);
            this.groupBox4.TabIndex = 201;
            this.groupBox4.TabStop = false;
            this.groupBox4.Visible = false;
            // 
            // textBox6
            // 
            this.textBox6.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.textBox6.Location = new System.Drawing.Point(6, 17);
            this.textBox6.Multiline = true;
            this.textBox6.Name = "textBox6";
            this.textBox6.ReadOnly = true;
            this.textBox6.Size = new System.Drawing.Size(87, 22);
            this.textBox6.TabIndex = 196;
            this.textBox6.Text = "UTM zone (1-60)";
            this.textBox6.Visible = false;
            // 
            // UTMsouthcheck
            // 
            this.UTMsouthcheck.AutoSize = true;
            this.UTMsouthcheck.Location = new System.Drawing.Point(6, 42);
            this.UTMsouthcheck.Name = "UTMsouthcheck";
            this.UTMsouthcheck.Size = new System.Drawing.Size(128, 17);
            this.UTMsouthcheck.TabIndex = 197;
            this.UTMsouthcheck.Text = "Southern Hemisphere";
            this.UTMsouthcheck.UseVisualStyleBackColor = true;
            this.UTMsouthcheck.Visible = false;
            // 
            // UTMzonebox
            // 
            this.UTMzonebox.Location = new System.Drawing.Point(99, 16);
            this.UTMzonebox.Name = "UTMzonebox";
            this.UTMzonebox.Size = new System.Drawing.Size(39, 20);
            this.UTMzonebox.TabIndex = 194;
            this.UTMzonebox.Tag = "UTM zone";
            this.toolTip1.SetToolTip(this.UTMzonebox, "Enter the UTM zone");
            this.UTMzonebox.Visible = false;
            // 
            // UTMgridcheckbox
            // 
            this.UTMgridcheckbox.AutoSize = true;
            this.UTMgridcheckbox.Location = new System.Drawing.Point(225, 116);
            this.UTMgridcheckbox.Name = "UTMgridcheckbox";
            this.UTMgridcheckbox.Size = new System.Drawing.Size(82, 17);
            this.UTMgridcheckbox.TabIndex = 200;
            this.UTMgridcheckbox.Text = "Grid is UTM";
            this.UTMgridcheckbox.UseVisualStyleBackColor = true;
            this.UTMgridcheckbox.CheckedChanged += new System.EventHandler(this.UTMgridcheckbox_CheckedChanged);
            // 
            // textBox5
            // 
            this.textBox5.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.textBox5.Location = new System.Drawing.Point(26, 112);
            this.textBox5.Multiline = true;
            this.textBox5.Name = "textBox5";
            this.textBox5.ReadOnly = true;
            this.textBox5.Size = new System.Drawing.Size(190, 35);
            this.textBox5.TabIndex = 199;
            this.textBox5.Text = "For DTMs using British National Grid or UTM WGS84";
            // 
            // googleBeginDate
            // 
            this.googleBeginDate.AcceptsTab = true;
            this.googleBeginDate.Location = new System.Drawing.Point(222, 84);
            this.googleBeginDate.Name = "googleBeginDate";
            this.googleBeginDate.Size = new System.Drawing.Size(106, 20);
            this.googleBeginDate.TabIndex = 191;
            // 
            // label78
            // 
            this.label78.AutoSize = true;
            this.label78.Location = new System.Drawing.Point(96, 87);
            this.label78.Name = "label78";
            this.label78.Size = new System.Drawing.Size(120, 13);
            this.label78.TabIndex = 190;
            this.label78.Text = "begin date (yyyy-mm-dd)";
            // 
            // googAnimationSaveInterval
            // 
            this.googAnimationSaveInterval.Location = new System.Drawing.Point(222, 58);
            this.googAnimationSaveInterval.Name = "googAnimationSaveInterval";
            this.googAnimationSaveInterval.Size = new System.Drawing.Size(56, 20);
            this.googAnimationSaveInterval.TabIndex = 188;
            this.googAnimationSaveInterval.Text = "1000";
            // 
            // label79
            // 
            this.label79.Location = new System.Drawing.Point(64, 58);
            this.label79.Name = "label79";
            this.label79.Size = new System.Drawing.Size(152, 25);
            this.label79.TabIndex = 189;
            this.label79.Text = "Save file every * mins";
            this.label79.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.toolTip1.SetToolTip(this.label79, "How often the avi file AND the other data files are saved");
            // 
            // googleAnimationTextBox
            // 
            this.googleAnimationTextBox.Location = new System.Drawing.Point(222, 32);
            this.googleAnimationTextBox.Name = "googleAnimationTextBox";
            this.googleAnimationTextBox.Size = new System.Drawing.Size(106, 20);
            this.googleAnimationTextBox.TabIndex = 185;
            this.googleAnimationTextBox.Text = "animation.kmz";
            this.toolTip1.SetToolTip(this.googleAnimationTextBox, "File name for avi file");
            // 
            // googleAnimationCheckbox
            // 
            this.googleAnimationCheckbox.Location = new System.Drawing.Point(27, 31);
            this.googleAnimationCheckbox.Name = "googleAnimationCheckbox";
            this.googleAnimationCheckbox.Size = new System.Drawing.Size(195, 24);
            this.googleAnimationCheckbox.TabIndex = 184;
            this.googleAnimationCheckbox.Text = "Generate Google Earth Animation";
            this.toolTip1.SetToolTip(this.googleAnimationCheckbox, "Check to generate a movie file of the screen display");
            // 
            // checkBox3
            // 
            this.checkBox3.Location = new System.Drawing.Point(307, 5);
            this.checkBox3.Name = "checkBox3";
            this.checkBox3.Size = new System.Drawing.Size(147, 42);
            this.checkBox3.TabIndex = 204;
            this.checkBox3.Text = "Stage/Tidal input";
            this.toolTip1.SetToolTip(this.checkBox3, "CAESAR can run in both catchment and reach mode combined. But if you have reach m" +
        "ode selected, you should enter an input file in the hydrology tab");
            // 
            // outputfilesaveintervalbox
            // 
            this.outputfilesaveintervalbox.Location = new System.Drawing.Point(208, 327);
            this.outputfilesaveintervalbox.Name = "outputfilesaveintervalbox";
            this.outputfilesaveintervalbox.Size = new System.Drawing.Size(56, 20);
            this.outputfilesaveintervalbox.TabIndex = 166;
            this.outputfilesaveintervalbox.Text = "60";
            // 
            // TimeseriesOutBox
            // 
            this.TimeseriesOutBox.Location = new System.Drawing.Point(208, 303);
            this.TimeseriesOutBox.Name = "TimeseriesOutBox";
            this.TimeseriesOutBox.Size = new System.Drawing.Size(112, 20);
            this.TimeseriesOutBox.TabIndex = 168;
            this.TimeseriesOutBox.Text = "catchment.dat";
            // 
            // tracerOutputtextBox
            // 
            this.tracerOutputtextBox.Location = new System.Drawing.Point(760, 55);
            this.tracerOutputtextBox.Name = "tracerOutputtextBox";
            this.tracerOutputtextBox.Size = new System.Drawing.Size(112, 20);
            this.tracerOutputtextBox.TabIndex = 181;
            this.tracerOutputtextBox.Text = "tracer_output.txt";
            this.tracerOutputtextBox.Visible = false;
            // 
            // tracerOutcheckBox
            // 
            this.tracerOutcheckBox.Location = new System.Drawing.Point(720, 81);
            this.tracerOutcheckBox.Name = "tracerOutcheckBox";
            this.tracerOutcheckBox.Size = new System.Drawing.Size(160, 24);
            this.tracerOutcheckBox.TabIndex = 180;
            this.tracerOutcheckBox.Text = "Generate tracer output";
            this.tracerOutcheckBox.Visible = false;
            // 
            // reach_mode_box
            // 
            this.reach_mode_box.Checked = true;
            this.reach_mode_box.CheckState = System.Windows.Forms.CheckState.Checked;
            this.reach_mode_box.Location = new System.Drawing.Point(196, 5);
            this.reach_mode_box.Name = "reach_mode_box";
            this.reach_mode_box.Size = new System.Drawing.Size(147, 42);
            this.reach_mode_box.TabIndex = 175;
            this.reach_mode_box.Text = "Reach Mode";
            this.toolTip1.SetToolTip(this.reach_mode_box, "CAESAR can run in both catchment and reach mode combined. But if you have reach m" +
        "ode selected, you should enter an input file in the hydrology tab");
            // 
            // catchment_mode_box
            // 
            this.catchment_mode_box.Location = new System.Drawing.Point(54, 9);
            this.catchment_mode_box.Name = "catchment_mode_box";
            this.catchment_mode_box.Size = new System.Drawing.Size(147, 34);
            this.catchment_mode_box.TabIndex = 174;
            this.catchment_mode_box.Text = "Catchment mode";
            this.toolTip1.SetToolTip(this.catchment_mode_box, "CAESAR can run in both catchment and reach mode, but if you have catchment mode c" +
        "hecked, you should input a rainfall data file");
            this.catchment_mode_box.CheckedChanged += new System.EventHandler(this.checkBox1_CheckedChanged);
            // 
            // checkBoxGenerateTimeSeries
            // 
            this.checkBoxGenerateTimeSeries.Location = new System.Drawing.Point(44, 304);
            this.checkBoxGenerateTimeSeries.Name = "checkBoxGenerateTimeSeries";
            this.checkBoxGenerateTimeSeries.Size = new System.Drawing.Size(170, 24);
            this.checkBoxGenerateTimeSeries.TabIndex = 173;
            this.checkBoxGenerateTimeSeries.Text = "Generate time series output";
            // 
            // checkBoxGenerateIterations
            // 
            this.checkBoxGenerateIterations.Location = new System.Drawing.Point(45, 367);
            this.checkBoxGenerateIterations.Name = "checkBoxGenerateIterations";
            this.checkBoxGenerateIterations.Size = new System.Drawing.Size(152, 24);
            this.checkBoxGenerateIterations.TabIndex = 172;
            this.checkBoxGenerateIterations.Text = "Generate iteration output";
            this.checkBoxGenerateIterations.Visible = false;
            // 
            // IterationOutbox
            // 
            this.IterationOutbox.Location = new System.Drawing.Point(208, 367);
            this.IterationOutbox.Name = "IterationOutbox";
            this.IterationOutbox.Size = new System.Drawing.Size(112, 20);
            this.IterationOutbox.TabIndex = 170;
            this.IterationOutbox.Text = "iterout.dat";
            this.IterationOutbox.Visible = false;
            // 
            // saveintervalbox
            // 
            this.saveintervalbox.Location = new System.Drawing.Point(175, 224);
            this.saveintervalbox.Name = "saveintervalbox";
            this.saveintervalbox.Size = new System.Drawing.Size(56, 20);
            this.saveintervalbox.TabIndex = 163;
            this.saveintervalbox.Text = "1000";
            // 
            // label45
            // 
            this.label45.Location = new System.Drawing.Point(48, 327);
            this.label45.Name = "label45";
            this.label45.Size = new System.Drawing.Size(152, 24);
            this.label45.TabIndex = 167;
            this.label45.Text = "Save file every * mins";
            this.label45.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // label33
            // 
            this.label33.Location = new System.Drawing.Point(17, 221);
            this.label33.Name = "label33";
            this.label33.Size = new System.Drawing.Size(152, 25);
            this.label33.TabIndex = 165;
            this.label33.Text = "Save file every * mins";
            this.label33.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.toolTip1.SetToolTip(this.label33, "How often the avi file AND the other data files are saved");
            // 
            // uniquefilecheck
            // 
            this.uniquefilecheck.Location = new System.Drawing.Point(44, 245);
            this.uniquefilecheck.Name = "uniquefilecheck";
            this.uniquefilecheck.Size = new System.Drawing.Size(120, 25);
            this.uniquefilecheck.TabIndex = 164;
            this.uniquefilecheck.Text = "unique file name?";
            this.toolTip1.SetToolTip(this.uniquefilecheck, "Whether the data files are given a unqiu file name - ");
            // 
            // label32
            // 
            this.label32.Location = new System.Drawing.Point(603, 19);
            this.label32.Name = "label32";
            this.label32.Size = new System.Drawing.Size(128, 24);
            this.label32.TabIndex = 106;
            this.label32.Text = "tracer sediment vol file";
            this.label32.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.label32.Visible = false;
            // 
            // tracerhydrofile
            // 
            this.tracerhydrofile.Location = new System.Drawing.Point(656, 42);
            this.tracerhydrofile.Name = "tracerhydrofile";
            this.tracerhydrofile.Size = new System.Drawing.Size(120, 20);
            this.tracerhydrofile.TabIndex = 105;
            this.tracerhydrofile.Text = "null";
            this.tracerhydrofile.Visible = false;
            // 
            // tracerfile
            // 
            this.tracerfile.Location = new System.Drawing.Point(752, 16);
            this.tracerfile.Name = "tracerfile";
            this.tracerfile.Size = new System.Drawing.Size(120, 20);
            this.tracerfile.TabIndex = 104;
            this.tracerfile.Text = "null";
            this.tracerfile.Visible = false;
            // 
            // bedrockbox
            // 
            this.bedrockbox.Location = new System.Drawing.Point(131, 97);
            this.bedrockbox.Name = "bedrockbox";
            this.bedrockbox.Size = new System.Drawing.Size(120, 20);
            this.bedrockbox.TabIndex = 102;
            this.bedrockbox.Text = "null";
            // 
            // graindataloadbox
            // 
            this.graindataloadbox.Location = new System.Drawing.Point(131, 73);
            this.graindataloadbox.Name = "graindataloadbox";
            this.graindataloadbox.Size = new System.Drawing.Size(120, 20);
            this.graindataloadbox.TabIndex = 101;
            this.graindataloadbox.Text = "null";
            // 
            // openfiletextbox
            // 
            this.openfiletextbox.Location = new System.Drawing.Point(131, 49);
            this.openfiletextbox.Name = "openfiletextbox";
            this.openfiletextbox.Size = new System.Drawing.Size(120, 20);
            this.openfiletextbox.TabIndex = 100;
            this.openfiletextbox.Text = "whole9.dat";
            // 
            // tracerbox
            // 
            this.tracerbox.Location = new System.Drawing.Point(760, 16);
            this.tracerbox.Name = "tracerbox";
            this.tracerbox.Size = new System.Drawing.Size(88, 23);
            this.tracerbox.TabIndex = 99;
            this.tracerbox.Text = "tracer run?";
            this.toolTip1.SetToolTip(this.tracerbox, "Check to run in \'tracer mode\'");
            this.tracerbox.Visible = false;
            // 
            // label30
            // 
            this.label30.Location = new System.Drawing.Point(732, 16);
            this.label30.Name = "label30";
            this.label30.Size = new System.Drawing.Size(128, 24);
            this.label30.TabIndex = 98;
            this.label30.Text = "tracer input file";
            this.label30.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.label30.Visible = false;
            // 
            // label39
            // 
            this.label39.Location = new System.Drawing.Point(19, 97);
            this.label39.Name = "label39";
            this.label39.Size = new System.Drawing.Size(104, 24);
            this.label39.TabIndex = 96;
            this.label39.Text = "Bedrock data file";
            this.label39.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.toolTip1.SetToolTip(this.label39, "A DEM of the bedrock - below which the model cannot erode");
            // 
            // label24
            // 
            this.label24.Location = new System.Drawing.Point(19, 73);
            this.label24.Name = "label24";
            this.label24.Size = new System.Drawing.Size(104, 24);
            this.label24.TabIndex = 58;
            this.label24.Text = "Grain data file";
            this.label24.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // label23
            // 
            this.label23.Location = new System.Drawing.Point(19, 49);
            this.label23.Name = "label23";
            this.label23.Size = new System.Drawing.Size(104, 24);
            this.label23.TabIndex = 56;
            this.label23.Text = "DEM data file";
            this.label23.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // NumericalTab
            // 
            this.NumericalTab.Controls.Add(this.bedslopebox2);
            this.NumericalTab.Controls.Add(this.max_time_step_Box);
            this.NumericalTab.Controls.Add(this.label76);
            this.NumericalTab.Controls.Add(this.veltaubox);
            this.NumericalTab.Controls.Add(this.label52);
            this.NumericalTab.Controls.Add(this.bedslope_box);
            this.NumericalTab.Controls.Add(this.label47);
            this.NumericalTab.Controls.Add(this.mintimestepbox);
            this.NumericalTab.Controls.Add(this.textBox1);
            this.NumericalTab.Controls.Add(this.smoothbox);
            this.NumericalTab.Controls.Add(this.cyclemaxbox);
            this.NumericalTab.Controls.Add(this.itermaxbox);
            this.NumericalTab.Controls.Add(this.limitbox);
            this.NumericalTab.Controls.Add(this.label31);
            this.NumericalTab.Controls.Add(this.label27);
            this.NumericalTab.Controls.Add(this.label26);
            this.NumericalTab.Controls.Add(this.label10);
            this.NumericalTab.Controls.Add(this.label49);
            this.NumericalTab.Location = new System.Drawing.Point(4, 22);
            this.NumericalTab.Name = "NumericalTab";
            this.NumericalTab.Size = new System.Drawing.Size(1323, 487);
            this.NumericalTab.TabIndex = 2;
            this.NumericalTab.Text = "Numerical";
            this.NumericalTab.UseVisualStyleBackColor = true;
            // 
            // bedslopebox2
            // 
            this.bedslopebox2.Location = new System.Drawing.Point(84, 298);
            this.bedslopebox2.Name = "bedslopebox2";
            this.bedslopebox2.Size = new System.Drawing.Size(153, 35);
            this.bedslopebox2.TabIndex = 189;
            this.bedslopebox2.Text = "redundant now hidden";
            this.bedslopebox2.Visible = false;
            this.bedslopebox2.CheckedChanged += new System.EventHandler(this.bedslopebox2_CheckedChanged);
            // 
            // max_time_step_Box
            // 
            this.max_time_step_Box.Location = new System.Drawing.Point(184, 80);
            this.max_time_step_Box.Name = "max_time_step_Box";
            this.max_time_step_Box.Size = new System.Drawing.Size(64, 20);
            this.max_time_step_Box.TabIndex = 186;
            this.max_time_step_Box.Text = "3600";
            // 
            // label76
            // 
            this.label76.Location = new System.Drawing.Point(40, 80);
            this.label76.Name = "label76";
            this.label76.Size = new System.Drawing.Size(136, 24);
            this.label76.TabIndex = 187;
            this.label76.Text = "Max time step (secs)";
            this.label76.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // veltaubox
            // 
            this.veltaubox.Location = new System.Drawing.Point(77, 368);
            this.veltaubox.Name = "veltaubox";
            this.veltaubox.Size = new System.Drawing.Size(160, 27);
            this.veltaubox.TabIndex = 183;
            this.veltaubox.Text = "Tau based on velocity";
            this.veltaubox.Visible = false;
            this.veltaubox.CheckedChanged += new System.EventHandler(this.veltaubox_CheckedChanged);
            // 
            // label52
            // 
            this.label52.Location = new System.Drawing.Point(81, 342);
            this.label52.Name = "label52";
            this.label52.Size = new System.Drawing.Size(133, 28);
            this.label52.TabIndex = 182;
            this.label52.Text = "Slope used to calc Tau (for erosion)";
            this.toolTip1.SetToolTip(this.label52, "See notes on lateral tab for more. ");
            this.label52.Visible = false;
            // 
            // bedslope_box
            // 
            this.bedslope_box.Checked = true;
            this.bedslope_box.CheckState = System.Windows.Forms.CheckState.Checked;
            this.bedslope_box.Location = new System.Drawing.Point(22, 345);
            this.bedslope_box.Name = "bedslope_box";
            this.bedslope_box.Size = new System.Drawing.Size(192, 35);
            this.bedslope_box.TabIndex = 180;
            this.bedslope_box.Text = "Bedslope (original method)";
            this.bedslope_box.Visible = false;
            this.bedslope_box.CheckedChanged += new System.EventHandler(this.bedslope_box_CheckedChanged);
            // 
            // label47
            // 
            this.label47.Location = new System.Drawing.Point(256, 53);
            this.label47.Name = "label47";
            this.label47.Size = new System.Drawing.Size(66, 23);
            this.label47.TabIndex = 176;
            this.label47.Text = "(caution !)";
            this.label47.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // mintimestepbox
            // 
            this.mintimestepbox.Location = new System.Drawing.Point(184, 53);
            this.mintimestepbox.Name = "mintimestepbox";
            this.mintimestepbox.Size = new System.Drawing.Size(64, 20);
            this.mintimestepbox.TabIndex = 166;
            this.mintimestepbox.Text = "1";
            // 
            // textBox1
            // 
            this.textBox1.Location = new System.Drawing.Point(184, 118);
            this.textBox1.Name = "textBox1";
            this.textBox1.Size = new System.Drawing.Size(64, 20);
            this.textBox1.TabIndex = 164;
            this.textBox1.Text = "0";
            // 
            // smoothbox
            // 
            this.smoothbox.Location = new System.Drawing.Point(163, 360);
            this.smoothbox.Name = "smoothbox";
            this.smoothbox.Size = new System.Drawing.Size(40, 20);
            this.smoothbox.TabIndex = 160;
            this.smoothbox.Text = "1";
            this.smoothbox.Visible = false;
            // 
            // cyclemaxbox
            // 
            this.cyclemaxbox.Location = new System.Drawing.Point(184, 142);
            this.cyclemaxbox.Name = "cyclemaxbox";
            this.cyclemaxbox.Size = new System.Drawing.Size(64, 20);
            this.cyclemaxbox.TabIndex = 155;
            this.cyclemaxbox.Text = "1000";
            // 
            // itermaxbox
            // 
            this.itermaxbox.Location = new System.Drawing.Point(200, 318);
            this.itermaxbox.Name = "itermaxbox";
            this.itermaxbox.Size = new System.Drawing.Size(64, 20);
            this.itermaxbox.TabIndex = 154;
            this.itermaxbox.Text = "100000";
            this.itermaxbox.Visible = false;
            // 
            // limitbox
            // 
            this.limitbox.Location = new System.Drawing.Point(184, 179);
            this.limitbox.Name = "limitbox";
            this.limitbox.Size = new System.Drawing.Size(40, 20);
            this.limitbox.TabIndex = 144;
            this.limitbox.Text = "1";
            // 
            // label31
            // 
            this.label31.Location = new System.Drawing.Point(56, 118);
            this.label31.Name = "label31";
            this.label31.Size = new System.Drawing.Size(120, 24);
            this.label31.TabIndex = 165;
            this.label31.Text = "run start time (h)";
            this.label31.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // label27
            // 
            this.label27.Location = new System.Drawing.Point(64, 142);
            this.label27.Name = "label27";
            this.label27.Size = new System.Drawing.Size(112, 24);
            this.label27.TabIndex = 157;
            this.label27.Text = "max run duration (h)";
            this.label27.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // label26
            // 
            this.label26.Location = new System.Drawing.Point(56, 318);
            this.label26.Name = "label26";
            this.label26.Size = new System.Drawing.Size(136, 24);
            this.label26.TabIndex = 156;
            this.label26.Text = "max # of iterations";
            this.label26.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.label26.Visible = false;
            // 
            // label10
            // 
            this.label10.Location = new System.Drawing.Point(56, 175);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(120, 24);
            this.label10.TabIndex = 152;
            this.label10.Text = "memory limit";
            this.label10.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // label49
            // 
            this.label49.Location = new System.Drawing.Point(40, 53);
            this.label49.Name = "label49";
            this.label49.Size = new System.Drawing.Size(136, 24);
            this.label49.TabIndex = 167;
            this.label49.Text = "Min time step (s)";
            this.label49.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // GrainTab
            // 
            this.GrainTab.Controls.Add(this.meyerbox);
            this.GrainTab.Controls.Add(this.checkBox8);
            this.GrainTab.Controls.Add(this.bedrock_erosion_threshold_box);
            this.GrainTab.Controls.Add(this.bedrock_erosion_rate_box);
            this.GrainTab.Controls.Add(this.label92);
            this.GrainTab.Controls.Add(this.label93);
            this.GrainTab.Controls.Add(this.label65);
            this.GrainTab.Controls.Add(this.textBox7);
            this.GrainTab.Controls.Add(this.label58);
            this.GrainTab.Controls.Add(this.downstreamshiftbox);
            this.GrainTab.Controls.Add(this.label55);
            this.GrainTab.Controls.Add(this.max_vel_box);
            this.GrainTab.Controls.Add(this.textBox3);
            this.GrainTab.Controls.Add(this.label60);
            this.GrainTab.Controls.Add(this.avge_smoothbox);
            this.GrainTab.Controls.Add(this.nolateral);
            this.GrainTab.Controls.Add(this.newlateral);
            this.GrainTab.Controls.Add(this.label7);
            this.GrainTab.Controls.Add(this.lateralratebox);
            this.GrainTab.Controls.Add(this.label48);
            this.GrainTab.Controls.Add(this.label54);
            this.GrainTab.Controls.Add(this.propremaining);
            this.GrainTab.Controls.Add(this.label50);
            this.GrainTab.Controls.Add(this.activebox);
            this.GrainTab.Controls.Add(this.erodefactorbox);
            this.GrainTab.Controls.Add(this.label12);
            this.GrainTab.Controls.Add(this.einsteinbox);
            this.GrainTab.Controls.Add(this.wilcockbox);
            this.GrainTab.Controls.Add(this.fallGS9box);
            this.GrainTab.Controls.Add(this.fallGS8box);
            this.GrainTab.Controls.Add(this.fallGS7box);
            this.GrainTab.Controls.Add(this.fallGS6box);
            this.GrainTab.Controls.Add(this.fallGS5box);
            this.GrainTab.Controls.Add(this.fallGS4box);
            this.GrainTab.Controls.Add(this.fallGS3box);
            this.GrainTab.Controls.Add(this.gpSumLabel);
            this.GrainTab.Controls.Add(this.gpSumLabel2);
            this.GrainTab.Controls.Add(this.suspGS9box);
            this.GrainTab.Controls.Add(this.suspGS8box);
            this.GrainTab.Controls.Add(this.suspGS7box);
            this.GrainTab.Controls.Add(this.suspGS6box);
            this.GrainTab.Controls.Add(this.suspGS5box);
            this.GrainTab.Controls.Add(this.suspGS4box);
            this.GrainTab.Controls.Add(this.suspGS3box);
            this.GrainTab.Controls.Add(this.suspGS2box);
            this.GrainTab.Controls.Add(this.fallGS2box);
            this.GrainTab.Controls.Add(this.fallGS1box);
            this.GrainTab.Controls.Add(this.label28);
            this.GrainTab.Controls.Add(this.label4);
            this.GrainTab.Controls.Add(this.suspGS1box);
            this.GrainTab.Controls.Add(this.gp3box);
            this.GrainTab.Controls.Add(this.gp4box);
            this.GrainTab.Controls.Add(this.gp5box);
            this.GrainTab.Controls.Add(this.gp6box);
            this.GrainTab.Controls.Add(this.gp7box);
            this.GrainTab.Controls.Add(this.gp8box);
            this.GrainTab.Controls.Add(this.gp9box);
            this.GrainTab.Controls.Add(this.gp2box);
            this.GrainTab.Controls.Add(this.gp1box);
            this.GrainTab.Controls.Add(this.g3box);
            this.GrainTab.Controls.Add(this.g4box);
            this.GrainTab.Controls.Add(this.g5box);
            this.GrainTab.Controls.Add(this.g6box);
            this.GrainTab.Controls.Add(this.g7box);
            this.GrainTab.Controls.Add(this.g8box);
            this.GrainTab.Controls.Add(this.g9box);
            this.GrainTab.Controls.Add(this.g2box);
            this.GrainTab.Controls.Add(this.g1box);
            this.GrainTab.Controls.Add(this.label22);
            this.GrainTab.Controls.Add(this.label21);
            this.GrainTab.Controls.Add(this.label20);
            this.GrainTab.Controls.Add(this.label19);
            this.GrainTab.Controls.Add(this.label18);
            this.GrainTab.Controls.Add(this.label17);
            this.GrainTab.Controls.Add(this.label16);
            this.GrainTab.Controls.Add(this.label15);
            this.GrainTab.Controls.Add(this.label14);
            this.GrainTab.Controls.Add(this.label13);
            this.GrainTab.Controls.Add(this.label6);
            this.GrainTab.Location = new System.Drawing.Point(4, 22);
            this.GrainTab.Name = "GrainTab";
            this.GrainTab.Size = new System.Drawing.Size(1323, 487);
            this.GrainTab.TabIndex = 3;
            this.GrainTab.Text = "Sediment";
            this.GrainTab.UseVisualStyleBackColor = true;
            // 
            // meyerbox
            // 
            this.meyerbox.AutoSize = true;
            this.meyerbox.Location = new System.Drawing.Point(605, 71);
            this.meyerbox.Name = "meyerbox";
            this.meyerbox.Size = new System.Drawing.Size(114, 17);
            this.meyerbox.TabIndex = 107;
            this.meyerbox.Text = "Meyer Peter Muller";
            this.meyerbox.UseVisualStyleBackColor = true;
            this.meyerbox.CheckedChanged += new System.EventHandler(this.meyerbox_CheckedChanged);
            // 
            // checkBox8
            // 
            this.checkBox8.Checked = true;
            this.checkBox8.CheckState = System.Windows.Forms.CheckState.Checked;
            this.checkBox8.Location = new System.Drawing.Point(27, 322);
            this.checkBox8.Name = "checkBox8";
            this.checkBox8.Size = new System.Drawing.Size(169, 28);
            this.checkBox8.TabIndex = 218;
            this.checkBox8.Text = "All 9 grainsizes?";
            // 
            // bedrock_erosion_threshold_box
            // 
            this.bedrock_erosion_threshold_box.Location = new System.Drawing.Point(27, 362);
            this.bedrock_erosion_threshold_box.Name = "bedrock_erosion_threshold_box";
            this.bedrock_erosion_threshold_box.Size = new System.Drawing.Size(100, 20);
            this.bedrock_erosion_threshold_box.TabIndex = 214;
            this.bedrock_erosion_threshold_box.Text = "0";
            // 
            // bedrock_erosion_rate_box
            // 
            this.bedrock_erosion_rate_box.Location = new System.Drawing.Point(27, 395);
            this.bedrock_erosion_rate_box.Name = "bedrock_erosion_rate_box";
            this.bedrock_erosion_rate_box.Size = new System.Drawing.Size(100, 20);
            this.bedrock_erosion_rate_box.TabIndex = 215;
            this.bedrock_erosion_rate_box.Text = "0";
            // 
            // label92
            // 
            this.label92.Location = new System.Drawing.Point(133, 358);
            this.label92.Name = "label92";
            this.label92.Size = new System.Drawing.Size(167, 33);
            this.label92.TabIndex = 217;
            this.label92.Text = "Bedrock erosion threshold (Pa)";
            this.label92.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label93
            // 
            this.label93.Location = new System.Drawing.Point(138, 393);
            this.label93.Name = "label93";
            this.label93.Size = new System.Drawing.Size(162, 24);
            this.label93.TabIndex = 216;
            this.label93.Text = "Bedrock erosion rate (m/Pa/Yr)";
            this.label93.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label65
            // 
            this.label65.Location = new System.Drawing.Point(686, 415);
            this.label65.Name = "label65";
            this.label65.Size = new System.Drawing.Size(131, 27);
            this.label65.TabIndex = 213;
            this.label65.Text = "Max difference allowed in cross channel smoothing";
            // 
            // textBox7
            // 
            this.textBox7.Location = new System.Drawing.Point(605, 415);
            this.textBox7.Name = "textBox7";
            this.textBox7.Size = new System.Drawing.Size(73, 20);
            this.textBox7.TabIndex = 212;
            this.textBox7.Text = "0.0001";
            // 
            // label58
            // 
            this.label58.Location = new System.Drawing.Point(686, 384);
            this.label58.Name = "label58";
            this.label58.Size = new System.Drawing.Size(131, 27);
            this.label58.TabIndex = 211;
            this.label58.Text = "Number of cells to shift lat erosion downstream";
            // 
            // downstreamshiftbox
            // 
            this.downstreamshiftbox.Location = new System.Drawing.Point(605, 384);
            this.downstreamshiftbox.Name = "downstreamshiftbox";
            this.downstreamshiftbox.Size = new System.Drawing.Size(73, 20);
            this.downstreamshiftbox.TabIndex = 210;
            this.downstreamshiftbox.Text = "5";
            // 
            // label55
            // 
            this.label55.Location = new System.Drawing.Point(683, 103);
            this.label55.Name = "label55";
            this.label55.Size = new System.Drawing.Size(195, 27);
            this.label55.TabIndex = 209;
            this.label55.Text = "Max velocity used to calc Tau from vel.";
            // 
            // max_vel_box
            // 
            this.max_vel_box.Location = new System.Drawing.Point(603, 104);
            this.max_vel_box.Name = "max_vel_box";
            this.max_vel_box.Size = new System.Drawing.Size(40, 20);
            this.max_vel_box.TabIndex = 208;
            this.max_vel_box.Text = "5";
            // 
            // textBox3
            // 
            this.textBox3.Location = new System.Drawing.Point(604, 324);
            this.textBox3.Name = "textBox3";
            this.textBox3.Size = new System.Drawing.Size(40, 20);
            this.textBox3.TabIndex = 202;
            this.textBox3.Text = "0";
            // 
            // label60
            // 
            this.label60.Location = new System.Drawing.Point(683, 350);
            this.label60.Name = "label60";
            this.label60.Size = new System.Drawing.Size(131, 27);
            this.label60.TabIndex = 201;
            this.label60.Text = "Number of passes for edge smoothing filter";
            // 
            // avge_smoothbox
            // 
            this.avge_smoothbox.Location = new System.Drawing.Point(604, 353);
            this.avge_smoothbox.Name = "avge_smoothbox";
            this.avge_smoothbox.Size = new System.Drawing.Size(73, 20);
            this.avge_smoothbox.TabIndex = 200;
            this.avge_smoothbox.Text = "100";
            // 
            // nolateral
            // 
            this.nolateral.Checked = true;
            this.nolateral.CheckState = System.Windows.Forms.CheckState.Checked;
            this.nolateral.Location = new System.Drawing.Point(386, 334);
            this.nolateral.Name = "nolateral";
            this.nolateral.Size = new System.Drawing.Size(121, 28);
            this.nolateral.TabIndex = 199;
            this.nolateral.Text = "No Lateral erosion";
            this.nolateral.Visible = false;
            // 
            // newlateral
            // 
            this.newlateral.Location = new System.Drawing.Point(606, 290);
            this.newlateral.Name = "newlateral";
            this.newlateral.Size = new System.Drawing.Size(106, 28);
            this.newlateral.TabIndex = 198;
            this.newlateral.Text = "Lateral Erosion";
            // 
            // label7
            // 
            this.label7.Location = new System.Drawing.Point(671, 323);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(95, 24);
            this.label7.TabIndex = 197;
            this.label7.Text = "Lat erosion rate";
            this.label7.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.toolTip1.SetToolTip(this.label7, "Lateral erosion constant that is applied to method 1 (old laterla) and method 2 (" +
        "new lateral)");
            // 
            // lateralratebox
            // 
            this.lateralratebox.Location = new System.Drawing.Point(603, 256);
            this.lateralratebox.Name = "lateralratebox";
            this.lateralratebox.Size = new System.Drawing.Size(64, 20);
            this.lateralratebox.TabIndex = 196;
            this.lateralratebox.Text = "20";
            // 
            // label48
            // 
            this.label48.Location = new System.Drawing.Point(683, 253);
            this.label48.Name = "label48";
            this.label48.Size = new System.Drawing.Size(152, 24);
            this.label48.TabIndex = 195;
            this.label48.Text = "in channel lateral erosion rate";
            this.label48.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // label54
            // 
            this.label54.Location = new System.Drawing.Point(683, 214);
            this.label54.Name = "label54";
            this.label54.Size = new System.Drawing.Size(195, 27);
            this.label54.TabIndex = 193;
            this.label54.Text = "Proportion re-circulated if recirculate box is checked";
            // 
            // propremaining
            // 
            this.propremaining.Location = new System.Drawing.Point(603, 217);
            this.propremaining.Name = "propremaining";
            this.propremaining.Size = new System.Drawing.Size(64, 20);
            this.propremaining.TabIndex = 192;
            this.propremaining.Text = "1.0";
            // 
            // label50
            // 
            this.label50.Location = new System.Drawing.Point(678, 154);
            this.label50.Name = "label50";
            this.label50.Size = new System.Drawing.Size(172, 48);
            this.label50.TabIndex = 173;
            this.label50.Text = "Active layer thickness (m) must be at least 4 times max erode limit";
            this.label50.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // activebox
            // 
            this.activebox.Location = new System.Drawing.Point(603, 160);
            this.activebox.Name = "activebox";
            this.activebox.Size = new System.Drawing.Size(64, 20);
            this.activebox.TabIndex = 172;
            this.activebox.Text = "0.1";
            // 
            // erodefactorbox
            // 
            this.erodefactorbox.Location = new System.Drawing.Point(603, 133);
            this.erodefactorbox.Name = "erodefactorbox";
            this.erodefactorbox.Size = new System.Drawing.Size(40, 20);
            this.erodefactorbox.TabIndex = 170;
            this.erodefactorbox.Text = "0.02";
            // 
            // label12
            // 
            this.label12.Location = new System.Drawing.Point(673, 130);
            this.label12.Name = "label12";
            this.label12.Size = new System.Drawing.Size(87, 24);
            this.label12.TabIndex = 171;
            this.label12.Text = "Max erode limit";
            this.label12.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // einsteinbox
            // 
            this.einsteinbox.AutoSize = true;
            this.einsteinbox.Location = new System.Drawing.Point(605, 51);
            this.einsteinbox.Name = "einsteinbox";
            this.einsteinbox.Size = new System.Drawing.Size(63, 17);
            this.einsteinbox.TabIndex = 107;
            this.einsteinbox.Text = "Einstein";
            this.einsteinbox.UseVisualStyleBackColor = true;
            this.einsteinbox.CheckedChanged += new System.EventHandler(this.einsteinbox_CheckedChanged);
            // 
            // wilcockbox
            // 
            this.wilcockbox.AutoSize = true;
            this.wilcockbox.Checked = true;
            this.wilcockbox.CheckState = System.Windows.Forms.CheckState.Checked;
            this.wilcockbox.Location = new System.Drawing.Point(604, 32);
            this.wilcockbox.Name = "wilcockbox";
            this.wilcockbox.Size = new System.Drawing.Size(119, 17);
            this.wilcockbox.TabIndex = 106;
            this.wilcockbox.Text = "Wilcock and Crowe";
            this.wilcockbox.UseVisualStyleBackColor = true;
            this.wilcockbox.CheckedChanged += new System.EventHandler(this.wilcockbox_CheckedChanged);
            // 
            // fallGS9box
            // 
            this.fallGS9box.Enabled = false;
            this.fallGS9box.Location = new System.Drawing.Point(416, 378);
            this.fallGS9box.Name = "fallGS9box";
            this.fallGS9box.Size = new System.Drawing.Size(100, 20);
            this.fallGS9box.TabIndex = 90;
            this.fallGS9box.Text = "1.357";
            this.fallGS9box.Visible = false;
            // 
            // fallGS8box
            // 
            this.fallGS8box.Enabled = false;
            this.fallGS8box.Location = new System.Drawing.Point(376, 378);
            this.fallGS8box.Name = "fallGS8box";
            this.fallGS8box.Size = new System.Drawing.Size(100, 20);
            this.fallGS8box.TabIndex = 89;
            this.fallGS8box.Text = "0.959";
            this.fallGS8box.Visible = false;
            // 
            // fallGS7box
            // 
            this.fallGS7box.Enabled = false;
            this.fallGS7box.Location = new System.Drawing.Point(424, 343);
            this.fallGS7box.Name = "fallGS7box";
            this.fallGS7box.Size = new System.Drawing.Size(100, 20);
            this.fallGS7box.TabIndex = 88;
            this.fallGS7box.Text = "0.678";
            this.fallGS7box.Visible = false;
            // 
            // fallGS6box
            // 
            this.fallGS6box.Enabled = false;
            this.fallGS6box.Location = new System.Drawing.Point(386, 364);
            this.fallGS6box.Name = "fallGS6box";
            this.fallGS6box.Size = new System.Drawing.Size(100, 20);
            this.fallGS6box.TabIndex = 87;
            this.fallGS6box.Text = "0.479";
            this.fallGS6box.Visible = false;
            // 
            // fallGS5box
            // 
            this.fallGS5box.Enabled = false;
            this.fallGS5box.Location = new System.Drawing.Point(386, 364);
            this.fallGS5box.Name = "fallGS5box";
            this.fallGS5box.Size = new System.Drawing.Size(100, 20);
            this.fallGS5box.TabIndex = 86;
            this.fallGS5box.Text = "0.338";
            this.fallGS5box.Visible = false;
            // 
            // fallGS4box
            // 
            this.fallGS4box.Enabled = false;
            this.fallGS4box.Location = new System.Drawing.Point(393, 368);
            this.fallGS4box.Name = "fallGS4box";
            this.fallGS4box.Size = new System.Drawing.Size(100, 20);
            this.fallGS4box.TabIndex = 85;
            this.fallGS4box.Text = "0.237";
            this.fallGS4box.Visible = false;
            // 
            // fallGS3box
            // 
            this.fallGS3box.Enabled = false;
            this.fallGS3box.Location = new System.Drawing.Point(386, 364);
            this.fallGS3box.Name = "fallGS3box";
            this.fallGS3box.Size = new System.Drawing.Size(100, 20);
            this.fallGS3box.TabIndex = 84;
            this.fallGS3box.Text = "0.164";
            this.fallGS3box.Visible = false;
            // 
            // gpSumLabel
            // 
            this.gpSumLabel.Location = new System.Drawing.Point(229, 343);
            this.gpSumLabel.Name = "gpSumLabel";
            this.gpSumLabel.Size = new System.Drawing.Size(96, 16);
            this.gpSumLabel.TabIndex = 105;
            this.gpSumLabel.Text = "OK";
            this.gpSumLabel.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // gpSumLabel2
            // 
            this.gpSumLabel2.ForeColor = System.Drawing.SystemColors.ControlText;
            this.gpSumLabel2.Location = new System.Drawing.Point(229, 319);
            this.gpSumLabel2.Name = "gpSumLabel2";
            this.gpSumLabel2.Size = new System.Drawing.Size(112, 16);
            this.gpSumLabel2.TabIndex = 104;
            this.gpSumLabel2.Text = "sum must equal 1.0";
            this.gpSumLabel2.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // suspGS9box
            // 
            this.suspGS9box.Location = new System.Drawing.Point(401, 374);
            this.suspGS9box.Name = "suspGS9box";
            this.suspGS9box.Size = new System.Drawing.Size(16, 24);
            this.suspGS9box.TabIndex = 81;
            this.suspGS9box.Visible = false;
            this.suspGS9box.CheckedChanged += new System.EventHandler(this.suspCheckedChange);
            // 
            // suspGS8box
            // 
            this.suspGS8box.Location = new System.Drawing.Point(394, 368);
            this.suspGS8box.Name = "suspGS8box";
            this.suspGS8box.Size = new System.Drawing.Size(16, 24);
            this.suspGS8box.TabIndex = 80;
            this.suspGS8box.Visible = false;
            this.suspGS8box.CheckedChanged += new System.EventHandler(this.suspCheckedChange);
            // 
            // suspGS7box
            // 
            this.suspGS7box.Location = new System.Drawing.Point(394, 368);
            this.suspGS7box.Name = "suspGS7box";
            this.suspGS7box.Size = new System.Drawing.Size(16, 24);
            this.suspGS7box.TabIndex = 79;
            this.suspGS7box.Visible = false;
            this.suspGS7box.CheckedChanged += new System.EventHandler(this.suspCheckedChange);
            // 
            // suspGS6box
            // 
            this.suspGS6box.Location = new System.Drawing.Point(401, 372);
            this.suspGS6box.Name = "suspGS6box";
            this.suspGS6box.Size = new System.Drawing.Size(16, 24);
            this.suspGS6box.TabIndex = 78;
            this.suspGS6box.Visible = false;
            this.suspGS6box.CheckedChanged += new System.EventHandler(this.suspCheckedChange);
            // 
            // suspGS5box
            // 
            this.suspGS5box.Location = new System.Drawing.Point(416, 372);
            this.suspGS5box.Name = "suspGS5box";
            this.suspGS5box.Size = new System.Drawing.Size(16, 24);
            this.suspGS5box.TabIndex = 77;
            this.suspGS5box.Visible = false;
            this.suspGS5box.CheckedChanged += new System.EventHandler(this.suspCheckedChange);
            // 
            // suspGS4box
            // 
            this.suspGS4box.Location = new System.Drawing.Point(399, 368);
            this.suspGS4box.Name = "suspGS4box";
            this.suspGS4box.Size = new System.Drawing.Size(16, 24);
            this.suspGS4box.TabIndex = 76;
            this.suspGS4box.Visible = false;
            this.suspGS4box.CheckedChanged += new System.EventHandler(this.suspCheckedChange);
            // 
            // suspGS3box
            // 
            this.suspGS3box.Location = new System.Drawing.Point(423, 368);
            this.suspGS3box.Name = "suspGS3box";
            this.suspGS3box.Size = new System.Drawing.Size(16, 24);
            this.suspGS3box.TabIndex = 75;
            this.suspGS3box.Visible = false;
            this.suspGS3box.CheckedChanged += new System.EventHandler(this.suspCheckedChange);
            // 
            // suspGS2box
            // 
            this.suspGS2box.Location = new System.Drawing.Point(416, 371);
            this.suspGS2box.Name = "suspGS2box";
            this.suspGS2box.Size = new System.Drawing.Size(16, 24);
            this.suspGS2box.TabIndex = 74;
            this.suspGS2box.Visible = false;
            this.suspGS2box.CheckedChanged += new System.EventHandler(this.suspCheckedChange);
            // 
            // fallGS2box
            // 
            this.fallGS2box.Location = new System.Drawing.Point(386, 364);
            this.fallGS2box.Name = "fallGS2box";
            this.fallGS2box.Size = new System.Drawing.Size(100, 20);
            this.fallGS2box.TabIndex = 83;
            this.fallGS2box.Text = "0.109";
            this.fallGS2box.Visible = false;
            // 
            // fallGS1box
            // 
            this.fallGS1box.Location = new System.Drawing.Point(424, 40);
            this.fallGS1box.Name = "fallGS1box";
            this.fallGS1box.Size = new System.Drawing.Size(100, 20);
            this.fallGS1box.TabIndex = 82;
            this.fallGS1box.Text = "0.066";
            // 
            // label28
            // 
            this.label28.Location = new System.Drawing.Point(432, 16);
            this.label28.Name = "label28";
            this.label28.Size = new System.Drawing.Size(88, 16);
            this.label28.TabIndex = 95;
            this.label28.Text = "fall velocity (m/s)";
            this.label28.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label4
            // 
            this.label4.Location = new System.Drawing.Point(336, 16);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(96, 16);
            this.label4.TabIndex = 85;
            this.label4.Text = "suspended ?";
            this.label4.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // suspGS1box
            // 
            this.suspGS1box.Location = new System.Drawing.Point(376, 40);
            this.suspGS1box.Name = "suspGS1box";
            this.suspGS1box.Size = new System.Drawing.Size(16, 24);
            this.suspGS1box.TabIndex = 73;
            this.suspGS1box.CheckedChanged += new System.EventHandler(this.suspCheckedChange);
            // 
            // gp3box
            // 
            this.gp3box.Location = new System.Drawing.Point(232, 104);
            this.gp3box.Name = "gp3box";
            this.gp3box.Size = new System.Drawing.Size(100, 20);
            this.gp3box.TabIndex = 66;
            this.gp3box.Text = "0.019";
            this.gp3box.TextChanged += new System.EventHandler(this.fracGSchanged);
            // 
            // gp4box
            // 
            this.gp4box.Location = new System.Drawing.Point(232, 136);
            this.gp4box.Name = "gp4box";
            this.gp4box.Size = new System.Drawing.Size(100, 20);
            this.gp4box.TabIndex = 67;
            this.gp4box.Text = "0.029";
            this.gp4box.TextChanged += new System.EventHandler(this.fracGSchanged);
            // 
            // gp5box
            // 
            this.gp5box.Location = new System.Drawing.Point(232, 168);
            this.gp5box.Name = "gp5box";
            this.gp5box.Size = new System.Drawing.Size(100, 20);
            this.gp5box.TabIndex = 68;
            this.gp5box.Text = "0.068";
            this.gp5box.TextChanged += new System.EventHandler(this.fracGSchanged);
            // 
            // gp6box
            // 
            this.gp6box.Location = new System.Drawing.Point(232, 200);
            this.gp6box.Name = "gp6box";
            this.gp6box.Size = new System.Drawing.Size(100, 20);
            this.gp6box.TabIndex = 69;
            this.gp6box.Text = "0.146";
            this.gp6box.TextChanged += new System.EventHandler(this.fracGSchanged);
            // 
            // gp7box
            // 
            this.gp7box.Location = new System.Drawing.Point(232, 232);
            this.gp7box.Name = "gp7box";
            this.gp7box.Size = new System.Drawing.Size(100, 20);
            this.gp7box.TabIndex = 70;
            this.gp7box.Text = "0.220";
            this.gp7box.TextChanged += new System.EventHandler(this.fracGSchanged);
            // 
            // gp8box
            // 
            this.gp8box.Location = new System.Drawing.Point(232, 264);
            this.gp8box.Name = "gp8box";
            this.gp8box.Size = new System.Drawing.Size(100, 20);
            this.gp8box.TabIndex = 71;
            this.gp8box.Text = "0.231";
            this.gp8box.TextChanged += new System.EventHandler(this.fracGSchanged);
            // 
            // gp9box
            // 
            this.gp9box.Location = new System.Drawing.Point(232, 296);
            this.gp9box.Name = "gp9box";
            this.gp9box.Size = new System.Drawing.Size(100, 20);
            this.gp9box.TabIndex = 72;
            this.gp9box.Text = "0.121";
            this.gp9box.TextChanged += new System.EventHandler(this.fracGSchanged);
            // 
            // gp2box
            // 
            this.gp2box.Location = new System.Drawing.Point(232, 72);
            this.gp2box.Name = "gp2box";
            this.gp2box.Size = new System.Drawing.Size(100, 20);
            this.gp2box.TabIndex = 65;
            this.gp2box.Text = "0.022";
            this.gp2box.TextChanged += new System.EventHandler(this.fracGSchanged);
            // 
            // gp1box
            // 
            this.gp1box.Location = new System.Drawing.Point(232, 40);
            this.gp1box.Name = "gp1box";
            this.gp1box.Size = new System.Drawing.Size(100, 20);
            this.gp1box.TabIndex = 64;
            this.gp1box.Text = "0.144";
            this.gp1box.TextChanged += new System.EventHandler(this.fracGSchanged);
            // 
            // g3box
            // 
            this.g3box.Location = new System.Drawing.Point(96, 104);
            this.g3box.Name = "g3box";
            this.g3box.Size = new System.Drawing.Size(100, 20);
            this.g3box.TabIndex = 57;
            this.g3box.Text = "0.002";
            // 
            // g4box
            // 
            this.g4box.Location = new System.Drawing.Point(96, 136);
            this.g4box.Name = "g4box";
            this.g4box.Size = new System.Drawing.Size(100, 20);
            this.g4box.TabIndex = 58;
            this.g4box.Text = "0.004";
            // 
            // g5box
            // 
            this.g5box.Location = new System.Drawing.Point(96, 168);
            this.g5box.Name = "g5box";
            this.g5box.Size = new System.Drawing.Size(100, 20);
            this.g5box.TabIndex = 59;
            this.g5box.Text = "0.008";
            // 
            // g6box
            // 
            this.g6box.Location = new System.Drawing.Point(96, 200);
            this.g6box.Name = "g6box";
            this.g6box.Size = new System.Drawing.Size(100, 20);
            this.g6box.TabIndex = 60;
            this.g6box.Text = "0.016";
            // 
            // g7box
            // 
            this.g7box.Location = new System.Drawing.Point(96, 232);
            this.g7box.Name = "g7box";
            this.g7box.Size = new System.Drawing.Size(100, 20);
            this.g7box.TabIndex = 61;
            this.g7box.Text = "0.032";
            // 
            // g8box
            // 
            this.g8box.Location = new System.Drawing.Point(96, 264);
            this.g8box.Name = "g8box";
            this.g8box.Size = new System.Drawing.Size(100, 20);
            this.g8box.TabIndex = 62;
            this.g8box.Text = "0.064";
            // 
            // g9box
            // 
            this.g9box.Location = new System.Drawing.Point(96, 296);
            this.g9box.Name = "g9box";
            this.g9box.Size = new System.Drawing.Size(100, 20);
            this.g9box.TabIndex = 63;
            this.g9box.Text = "0.128";
            // 
            // g2box
            // 
            this.g2box.Location = new System.Drawing.Point(96, 72);
            this.g2box.Name = "g2box";
            this.g2box.Size = new System.Drawing.Size(100, 20);
            this.g2box.TabIndex = 56;
            this.g2box.Text = "0.001";
            // 
            // g1box
            // 
            this.g1box.Location = new System.Drawing.Point(96, 40);
            this.g1box.Name = "g1box";
            this.g1box.Size = new System.Drawing.Size(100, 20);
            this.g1box.TabIndex = 55;
            this.g1box.Text = "0.0005";
            // 
            // label22
            // 
            this.label22.Location = new System.Drawing.Point(232, 16);
            this.label22.Name = "label22";
            this.label22.Size = new System.Drawing.Size(96, 16);
            this.label22.TabIndex = 83;
            this.label22.Text = "proportion";
            this.label22.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label21
            // 
            this.label21.Location = new System.Drawing.Point(96, 16);
            this.label21.Name = "label21";
            this.label21.Size = new System.Drawing.Size(96, 16);
            this.label21.TabIndex = 82;
            this.label21.Text = "grain size (m)";
            this.label21.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label20
            // 
            this.label20.Location = new System.Drawing.Point(24, 72);
            this.label20.Name = "label20";
            this.label20.Size = new System.Drawing.Size(40, 24);
            this.label20.TabIndex = 81;
            this.label20.Text = "size2";
            this.label20.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label19
            // 
            this.label19.Location = new System.Drawing.Point(24, 104);
            this.label19.Name = "label19";
            this.label19.Size = new System.Drawing.Size(40, 24);
            this.label19.TabIndex = 80;
            this.label19.Text = "size3";
            this.label19.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label18
            // 
            this.label18.Location = new System.Drawing.Point(24, 136);
            this.label18.Name = "label18";
            this.label18.Size = new System.Drawing.Size(40, 24);
            this.label18.TabIndex = 79;
            this.label18.Text = "size4";
            this.label18.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label17
            // 
            this.label17.Location = new System.Drawing.Point(24, 168);
            this.label17.Name = "label17";
            this.label17.Size = new System.Drawing.Size(40, 24);
            this.label17.TabIndex = 78;
            this.label17.Text = "size5";
            this.label17.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label16
            // 
            this.label16.Location = new System.Drawing.Point(24, 200);
            this.label16.Name = "label16";
            this.label16.Size = new System.Drawing.Size(40, 24);
            this.label16.TabIndex = 77;
            this.label16.Text = "size6";
            this.label16.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label15
            // 
            this.label15.Location = new System.Drawing.Point(24, 232);
            this.label15.Name = "label15";
            this.label15.Size = new System.Drawing.Size(40, 24);
            this.label15.TabIndex = 76;
            this.label15.Text = "size7";
            this.label15.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label14
            // 
            this.label14.Location = new System.Drawing.Point(24, 264);
            this.label14.Name = "label14";
            this.label14.Size = new System.Drawing.Size(40, 24);
            this.label14.TabIndex = 75;
            this.label14.Text = "size8";
            this.label14.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label13
            // 
            this.label13.Location = new System.Drawing.Point(24, 296);
            this.label13.Name = "label13";
            this.label13.Size = new System.Drawing.Size(40, 24);
            this.label13.TabIndex = 74;
            this.label13.Text = "size9";
            this.label13.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label6
            // 
            this.label6.Location = new System.Drawing.Point(24, 40);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(40, 24);
            this.label6.TabIndex = 73;
            this.label6.Text = "size1";
            this.label6.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // DescriptionTab
            // 
            this.DescriptionTab.Controls.Add(this.DescBox);
            this.DescriptionTab.Location = new System.Drawing.Point(4, 22);
            this.DescriptionTab.Name = "DescriptionTab";
            this.DescriptionTab.Size = new System.Drawing.Size(1323, 487);
            this.DescriptionTab.TabIndex = 5;
            this.DescriptionTab.Text = "Description";
            this.DescriptionTab.UseVisualStyleBackColor = true;
            // 
            // DescBox
            // 
            this.DescBox.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.DescBox.Location = new System.Drawing.Point(16, 16);
            this.DescBox.Multiline = true;
            this.DescBox.Name = "DescBox";
            this.DescBox.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.DescBox.Size = new System.Drawing.Size(1126, 664);
            this.DescBox.TabIndex = 132;
            // 
            // GridTab
            // 
            this.GridTab.Controls.Add(this.overrideheaderBox);
            this.GridTab.Controls.Add(this.dxbox);
            this.GridTab.Controls.Add(this.label11);
            this.GridTab.Controls.Add(this.ytextbox);
            this.GridTab.Controls.Add(this.xtextbox);
            this.GridTab.Controls.Add(this.label2);
            this.GridTab.Controls.Add(this.label1);
            this.GridTab.Location = new System.Drawing.Point(4, 22);
            this.GridTab.Name = "GridTab";
            this.GridTab.Size = new System.Drawing.Size(1323, 487);
            this.GridTab.TabIndex = 1;
            this.GridTab.Text = "Grid";
            this.GridTab.UseVisualStyleBackColor = true;
            // 
            // overrideheaderBox
            // 
            this.overrideheaderBox.Location = new System.Drawing.Point(16, 16);
            this.overrideheaderBox.Name = "overrideheaderBox";
            this.overrideheaderBox.Size = new System.Drawing.Size(200, 24);
            this.overrideheaderBox.TabIndex = 27;
            this.overrideheaderBox.Text = "override header file";
            this.overrideheaderBox.CheckedChanged += new System.EventHandler(this.overrideheaderBox_CheckedChanged);
            // 
            // dxbox
            // 
            this.dxbox.Enabled = false;
            this.dxbox.Location = new System.Drawing.Point(120, 88);
            this.dxbox.Name = "dxbox";
            this.dxbox.Size = new System.Drawing.Size(40, 20);
            this.dxbox.TabIndex = 25;
            this.dxbox.Text = "5";
            // 
            // label11
            // 
            this.label11.Enabled = false;
            this.label11.Location = new System.Drawing.Point(16, 88);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(96, 24);
            this.label11.TabIndex = 26;
            this.label11.Text = "Cell size";
            this.label11.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // ytextbox
            // 
            this.ytextbox.Enabled = false;
            this.ytextbox.Location = new System.Drawing.Point(120, 64);
            this.ytextbox.Name = "ytextbox";
            this.ytextbox.Size = new System.Drawing.Size(56, 20);
            this.ytextbox.TabIndex = 5;
            this.ytextbox.Text = "358";
            // 
            // xtextbox
            // 
            this.xtextbox.Enabled = false;
            this.xtextbox.Location = new System.Drawing.Point(120, 40);
            this.xtextbox.Name = "xtextbox";
            this.xtextbox.Size = new System.Drawing.Size(56, 20);
            this.xtextbox.TabIndex = 4;
            this.xtextbox.Text = "593";
            // 
            // label2
            // 
            this.label2.Enabled = false;
            this.label2.Location = new System.Drawing.Point(16, 64);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(96, 24);
            this.label2.TabIndex = 7;
            this.label2.Text = "Y coordinates";
            this.label2.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // label1
            // 
            this.label1.Enabled = false;
            this.label1.Location = new System.Drawing.Point(24, 40);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(88, 24);
            this.label1.TabIndex = 6;
            this.label1.Text = "X coordinates";
            this.label1.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // HydrologyTab
            // 
            this.HydrologyTab.Controls.Add(this.groupBox7);
            this.HydrologyTab.Controls.Add(this.groupBox5);
            this.HydrologyTab.Controls.Add(this.groupBox1);
            this.HydrologyTab.Location = new System.Drawing.Point(4, 22);
            this.HydrologyTab.Name = "HydrologyTab";
            this.HydrologyTab.Size = new System.Drawing.Size(1323, 487);
            this.HydrologyTab.TabIndex = 4;
            this.HydrologyTab.Text = "Hydrology";
            this.HydrologyTab.UseVisualStyleBackColor = true;
            this.HydrologyTab.Click += new System.EventHandler(this.HydrologyTab_Click);
            // 
            // groupBox7
            // 
            this.groupBox7.Controls.Add(this.label105);
            this.groupBox7.Controls.Add(this.mfiletimestepbox);
            this.groupBox7.Controls.Add(this.hydroindexBox);
            this.groupBox7.Controls.Add(this.label103);
            this.groupBox7.Controls.Add(this.label37);
            this.groupBox7.Controls.Add(this.mvaluebox);
            this.groupBox7.Controls.Add(this.label102);
            this.groupBox7.Controls.Add(this.rfnumBox);
            this.groupBox7.Controls.Add(this.checkBox7);
            this.groupBox7.Controls.Add(this.label35);
            this.groupBox7.Controls.Add(this.raintimestepbox);
            this.groupBox7.Controls.Add(this.jmeaninputfilebox);
            this.groupBox7.Controls.Add(this.label59);
            this.groupBox7.Controls.Add(this.mvalueloadbox);
            this.groupBox7.Controls.Add(this.raindataloadbox);
            this.groupBox7.Controls.Add(this.label25);
            this.groupBox7.Location = new System.Drawing.Point(526, 17);
            this.groupBox7.Name = "groupBox7";
            this.groupBox7.Size = new System.Drawing.Size(467, 281);
            this.groupBox7.TabIndex = 222;
            this.groupBox7.TabStop = false;
            this.groupBox7.Text = "Rainfall input variables";
            // 
            // label105
            // 
            this.label105.Location = new System.Drawing.Point(288, 113);
            this.label105.Name = "label105";
            this.label105.Size = new System.Drawing.Size(104, 39);
            this.label105.TabIndex = 235;
            this.label105.Text = "Time varying M file time step (min)";
            this.label105.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // mfiletimestepbox
            // 
            this.mfiletimestepbox.Location = new System.Drawing.Point(398, 120);
            this.mfiletimestepbox.Name = "mfiletimestepbox";
            this.mfiletimestepbox.Size = new System.Drawing.Size(56, 20);
            this.mfiletimestepbox.TabIndex = 234;
            this.mfiletimestepbox.Text = "1440";
            // 
            // hydroindexBox
            // 
            this.hydroindexBox.Location = new System.Drawing.Point(152, 227);
            this.hydroindexBox.Name = "hydroindexBox";
            this.hydroindexBox.Size = new System.Drawing.Size(118, 20);
            this.hydroindexBox.TabIndex = 233;
            this.hydroindexBox.Text = "null";
            this.hydroindexBox.Visible = false;
            // 
            // label103
            // 
            this.label103.AutoSize = true;
            this.label103.Location = new System.Drawing.Point(40, 230);
            this.label103.Name = "label103";
            this.label103.Size = new System.Drawing.Size(74, 13);
            this.label103.TabIndex = 232;
            this.label103.Text = "hydroindex file";
            this.label103.Visible = false;
            // 
            // label37
            // 
            this.label37.Location = new System.Drawing.Point(16, 81);
            this.label37.Name = "label37";
            this.label37.Size = new System.Drawing.Size(128, 24);
            this.label37.TabIndex = 200;
            this.label37.Text = "\'m\' value";
            this.label37.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // mvaluebox
            // 
            this.mvaluebox.Location = new System.Drawing.Point(153, 85);
            this.mvaluebox.Name = "mvaluebox";
            this.mvaluebox.Size = new System.Drawing.Size(40, 20);
            this.mvaluebox.TabIndex = 199;
            this.mvaluebox.Text = "0.01";
            // 
            // label102
            // 
            this.label102.AutoSize = true;
            this.label102.Location = new System.Drawing.Point(40, 202);
            this.label102.Name = "label102";
            this.label102.Size = new System.Drawing.Size(98, 13);
            this.label102.TabIndex = 231;
            this.label102.Text = "number of rain cells";
            this.label102.Visible = false;
            // 
            // rfnumBox
            // 
            this.rfnumBox.Location = new System.Drawing.Point(152, 199);
            this.rfnumBox.Name = "rfnumBox";
            this.rfnumBox.Size = new System.Drawing.Size(56, 20);
            this.rfnumBox.TabIndex = 230;
            this.rfnumBox.Text = "1";
            this.rfnumBox.Visible = false;
            // 
            // checkBox7
            // 
            this.checkBox7.AutoSize = true;
            this.checkBox7.Location = new System.Drawing.Point(43, 177);
            this.checkBox7.Name = "checkBox7";
            this.checkBox7.Size = new System.Drawing.Size(297, 17);
            this.checkBox7.TabIndex = 229;
            this.checkBox7.Text = "Spatially variable rainfall and M value (if M value file used)";
            this.checkBox7.UseVisualStyleBackColor = true;
            this.checkBox7.CheckedChanged += new System.EventHandler(this.checkBox7_CheckedChanged);
            // 
            // label35
            // 
            this.label35.Location = new System.Drawing.Point(40, 47);
            this.label35.Name = "label35";
            this.label35.Size = new System.Drawing.Size(104, 39);
            this.label35.TabIndex = 228;
            this.label35.Text = "Rainfall data file time step (min)";
            this.label35.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // raintimestepbox
            // 
            this.raintimestepbox.Location = new System.Drawing.Point(153, 51);
            this.raintimestepbox.Name = "raintimestepbox";
            this.raintimestepbox.Size = new System.Drawing.Size(56, 20);
            this.raintimestepbox.TabIndex = 227;
            this.raintimestepbox.Text = "60";
            // 
            // jmeaninputfilebox
            // 
            this.jmeaninputfilebox.AutoSize = true;
            this.jmeaninputfilebox.Location = new System.Drawing.Point(43, 151);
            this.jmeaninputfilebox.Name = "jmeaninputfilebox";
            this.jmeaninputfilebox.Size = new System.Drawing.Size(263, 17);
            this.jmeaninputfilebox.TabIndex = 226;
            this.jmeaninputfilebox.Text = "if checked, discharge is read direct from rainfall file";
            this.jmeaninputfilebox.UseVisualStyleBackColor = true;
            // 
            // label59
            // 
            this.label59.AutoSize = true;
            this.label59.Location = new System.Drawing.Point(40, 126);
            this.label59.Name = "label59";
            this.label59.Size = new System.Drawing.Size(95, 13);
            this.label59.TabIndex = 225;
            this.label59.Text = "Time varying M file";
            // 
            // mvalueloadbox
            // 
            this.mvalueloadbox.Location = new System.Drawing.Point(153, 120);
            this.mvalueloadbox.Name = "mvalueloadbox";
            this.mvalueloadbox.Size = new System.Drawing.Size(118, 20);
            this.mvalueloadbox.TabIndex = 224;
            this.mvalueloadbox.Text = "null";
            // 
            // raindataloadbox
            // 
            this.raindataloadbox.Location = new System.Drawing.Point(152, 25);
            this.raindataloadbox.Name = "raindataloadbox";
            this.raindataloadbox.Size = new System.Drawing.Size(120, 20);
            this.raindataloadbox.TabIndex = 223;
            this.raindataloadbox.Text = "null";
            // 
            // label25
            // 
            this.label25.Location = new System.Drawing.Point(40, 25);
            this.label25.Name = "label25";
            this.label25.Size = new System.Drawing.Size(104, 24);
            this.label25.TabIndex = 222;
            this.label25.Text = "Rainfall data file";
            this.label25.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.toolTip1.SetToolTip(this.label25, "Hourly rainfall data - in an ascii format");
            // 
            // groupBox5
            // 
            this.groupBox5.Controls.Add(this.label83);
            this.groupBox5.Controls.Add(this.div_inputs_box);
            this.groupBox5.Controls.Add(this.infile8);
            this.groupBox5.Controls.Add(this.ybox8);
            this.groupBox5.Controls.Add(this.xbox8);
            this.groupBox5.Controls.Add(this.inbox8);
            this.groupBox5.Controls.Add(this.infile7);
            this.groupBox5.Controls.Add(this.ybox7);
            this.groupBox5.Controls.Add(this.xbox7);
            this.groupBox5.Controls.Add(this.inbox7);
            this.groupBox5.Controls.Add(this.infile6);
            this.groupBox5.Controls.Add(this.ybox6);
            this.groupBox5.Controls.Add(this.xbox6);
            this.groupBox5.Controls.Add(this.inbox6);
            this.groupBox5.Controls.Add(this.infile5);
            this.groupBox5.Controls.Add(this.ybox5);
            this.groupBox5.Controls.Add(this.xbox5);
            this.groupBox5.Controls.Add(this.inbox5);
            this.groupBox5.Controls.Add(this.input_time_step_box);
            this.groupBox5.Controls.Add(this.infile4);
            this.groupBox5.Controls.Add(this.infile3);
            this.groupBox5.Controls.Add(this.infile2);
            this.groupBox5.Controls.Add(this.infile1);
            this.groupBox5.Controls.Add(this.ybox1);
            this.groupBox5.Controls.Add(this.ybox2);
            this.groupBox5.Controls.Add(this.ybox3);
            this.groupBox5.Controls.Add(this.ybox4);
            this.groupBox5.Controls.Add(this.xbox2);
            this.groupBox5.Controls.Add(this.xbox3);
            this.groupBox5.Controls.Add(this.xbox4);
            this.groupBox5.Controls.Add(this.xbox1);
            this.groupBox5.Controls.Add(this.label29);
            this.groupBox5.Controls.Add(this.label44);
            this.groupBox5.Controls.Add(this.label43);
            this.groupBox5.Controls.Add(this.label41);
            this.groupBox5.Controls.Add(this.inbox2);
            this.groupBox5.Controls.Add(this.inbox3);
            this.groupBox5.Controls.Add(this.inbox4);
            this.groupBox5.Controls.Add(this.inbox1);
            this.groupBox5.Controls.Add(this.label42);
            this.groupBox5.Location = new System.Drawing.Point(32, 17);
            this.groupBox5.Name = "groupBox5";
            this.groupBox5.Size = new System.Drawing.Size(463, 332);
            this.groupBox5.TabIndex = 214;
            this.groupBox5.TabStop = false;
            this.groupBox5.Text = "Reach input variables";
            // 
            // label83
            // 
            this.label83.Location = new System.Drawing.Point(118, 275);
            this.label83.Name = "label83";
            this.label83.Size = new System.Drawing.Size(100, 32);
            this.label83.TabIndex = 202;
            this.label83.Text = "Divide inputs by..";
            // 
            // div_inputs_box
            // 
            this.div_inputs_box.Location = new System.Drawing.Point(224, 272);
            this.div_inputs_box.Name = "div_inputs_box";
            this.div_inputs_box.Size = new System.Drawing.Size(31, 20);
            this.div_inputs_box.TabIndex = 201;
            this.div_inputs_box.Text = "1";
            // 
            // infile8
            // 
            this.infile8.Location = new System.Drawing.Point(166, 236);
            this.infile8.Name = "infile8";
            this.infile8.Size = new System.Drawing.Size(120, 20);
            this.infile8.TabIndex = 198;
            // 
            // ybox8
            // 
            this.ybox8.Location = new System.Drawing.Point(120, 236);
            this.ybox8.Name = "ybox8";
            this.ybox8.Size = new System.Drawing.Size(31, 20);
            this.ybox8.TabIndex = 197;
            // 
            // xbox8
            // 
            this.xbox8.Location = new System.Drawing.Point(73, 236);
            this.xbox8.Name = "xbox8";
            this.xbox8.Size = new System.Drawing.Size(32, 20);
            this.xbox8.TabIndex = 196;
            // 
            // inbox8
            // 
            this.inbox8.Location = new System.Drawing.Point(9, 236);
            this.inbox8.Name = "inbox8";
            this.inbox8.Size = new System.Drawing.Size(64, 16);
            this.inbox8.TabIndex = 195;
            this.inbox8.Text = "Input 8";
            // 
            // infile7
            // 
            this.infile7.Location = new System.Drawing.Point(166, 208);
            this.infile7.Name = "infile7";
            this.infile7.Size = new System.Drawing.Size(120, 20);
            this.infile7.TabIndex = 194;
            // 
            // ybox7
            // 
            this.ybox7.Location = new System.Drawing.Point(120, 208);
            this.ybox7.Name = "ybox7";
            this.ybox7.Size = new System.Drawing.Size(31, 20);
            this.ybox7.TabIndex = 193;
            // 
            // xbox7
            // 
            this.xbox7.Location = new System.Drawing.Point(73, 208);
            this.xbox7.Name = "xbox7";
            this.xbox7.Size = new System.Drawing.Size(32, 20);
            this.xbox7.TabIndex = 192;
            // 
            // inbox7
            // 
            this.inbox7.Location = new System.Drawing.Point(9, 208);
            this.inbox7.Name = "inbox7";
            this.inbox7.Size = new System.Drawing.Size(64, 17);
            this.inbox7.TabIndex = 191;
            this.inbox7.Text = "Input 7";
            // 
            // infile6
            // 
            this.infile6.Location = new System.Drawing.Point(166, 180);
            this.infile6.Name = "infile6";
            this.infile6.Size = new System.Drawing.Size(120, 20);
            this.infile6.TabIndex = 190;
            // 
            // ybox6
            // 
            this.ybox6.Location = new System.Drawing.Point(120, 180);
            this.ybox6.Name = "ybox6";
            this.ybox6.Size = new System.Drawing.Size(31, 20);
            this.ybox6.TabIndex = 189;
            // 
            // xbox6
            // 
            this.xbox6.Location = new System.Drawing.Point(73, 180);
            this.xbox6.Name = "xbox6";
            this.xbox6.Size = new System.Drawing.Size(32, 20);
            this.xbox6.TabIndex = 188;
            // 
            // inbox6
            // 
            this.inbox6.Location = new System.Drawing.Point(9, 180);
            this.inbox6.Name = "inbox6";
            this.inbox6.Size = new System.Drawing.Size(64, 17);
            this.inbox6.TabIndex = 187;
            this.inbox6.Text = "Input 6";
            // 
            // infile5
            // 
            this.infile5.Location = new System.Drawing.Point(166, 153);
            this.infile5.Name = "infile5";
            this.infile5.Size = new System.Drawing.Size(120, 20);
            this.infile5.TabIndex = 186;
            // 
            // ybox5
            // 
            this.ybox5.Location = new System.Drawing.Point(120, 153);
            this.ybox5.Name = "ybox5";
            this.ybox5.Size = new System.Drawing.Size(31, 20);
            this.ybox5.TabIndex = 185;
            // 
            // xbox5
            // 
            this.xbox5.Location = new System.Drawing.Point(73, 153);
            this.xbox5.Name = "xbox5";
            this.xbox5.Size = new System.Drawing.Size(32, 20);
            this.xbox5.TabIndex = 184;
            // 
            // inbox5
            // 
            this.inbox5.Location = new System.Drawing.Point(9, 153);
            this.inbox5.Name = "inbox5";
            this.inbox5.Size = new System.Drawing.Size(64, 16);
            this.inbox5.TabIndex = 183;
            this.inbox5.Text = "Input 5";
            // 
            // input_time_step_box
            // 
            this.input_time_step_box.Location = new System.Drawing.Point(320, 90);
            this.input_time_step_box.Name = "input_time_step_box";
            this.input_time_step_box.Size = new System.Drawing.Size(63, 20);
            this.input_time_step_box.TabIndex = 181;
            this.input_time_step_box.Text = "1440";
            // 
            // infile4
            // 
            this.infile4.Location = new System.Drawing.Point(169, 127);
            this.infile4.Name = "infile4";
            this.infile4.Size = new System.Drawing.Size(120, 20);
            this.infile4.TabIndex = 180;
            // 
            // infile3
            // 
            this.infile3.Location = new System.Drawing.Point(169, 103);
            this.infile3.Name = "infile3";
            this.infile3.Size = new System.Drawing.Size(120, 20);
            this.infile3.TabIndex = 179;
            // 
            // infile2
            // 
            this.infile2.Location = new System.Drawing.Point(169, 79);
            this.infile2.Name = "infile2";
            this.infile2.Size = new System.Drawing.Size(120, 20);
            this.infile2.TabIndex = 178;
            // 
            // infile1
            // 
            this.infile1.Location = new System.Drawing.Point(169, 55);
            this.infile1.Name = "infile1";
            this.infile1.Size = new System.Drawing.Size(120, 20);
            this.infile1.TabIndex = 177;
            // 
            // ybox1
            // 
            this.ybox1.Location = new System.Drawing.Point(121, 55);
            this.ybox1.Name = "ybox1";
            this.ybox1.Size = new System.Drawing.Size(32, 20);
            this.ybox1.TabIndex = 175;
            // 
            // ybox2
            // 
            this.ybox2.Location = new System.Drawing.Point(121, 79);
            this.ybox2.Name = "ybox2";
            this.ybox2.Size = new System.Drawing.Size(32, 20);
            this.ybox2.TabIndex = 174;
            // 
            // ybox3
            // 
            this.ybox3.Location = new System.Drawing.Point(121, 103);
            this.ybox3.Name = "ybox3";
            this.ybox3.Size = new System.Drawing.Size(32, 20);
            this.ybox3.TabIndex = 173;
            // 
            // ybox4
            // 
            this.ybox4.Location = new System.Drawing.Point(121, 127);
            this.ybox4.Name = "ybox4";
            this.ybox4.Size = new System.Drawing.Size(32, 20);
            this.ybox4.TabIndex = 172;
            // 
            // xbox2
            // 
            this.xbox2.Location = new System.Drawing.Point(73, 79);
            this.xbox2.Name = "xbox2";
            this.xbox2.Size = new System.Drawing.Size(32, 20);
            this.xbox2.TabIndex = 171;
            // 
            // xbox3
            // 
            this.xbox3.Location = new System.Drawing.Point(73, 103);
            this.xbox3.Name = "xbox3";
            this.xbox3.Size = new System.Drawing.Size(32, 20);
            this.xbox3.TabIndex = 170;
            // 
            // xbox4
            // 
            this.xbox4.Location = new System.Drawing.Point(73, 127);
            this.xbox4.Name = "xbox4";
            this.xbox4.Size = new System.Drawing.Size(32, 20);
            this.xbox4.TabIndex = 169;
            // 
            // xbox1
            // 
            this.xbox1.Location = new System.Drawing.Point(73, 55);
            this.xbox1.Name = "xbox1";
            this.xbox1.Size = new System.Drawing.Size(32, 20);
            this.xbox1.TabIndex = 168;
            // 
            // label29
            // 
            this.label29.Location = new System.Drawing.Point(317, 60);
            this.label29.Name = "label29";
            this.label29.Size = new System.Drawing.Size(100, 32);
            this.label29.TabIndex = 182;
            this.label29.Text = "input data time step (mins)";
            // 
            // label44
            // 
            this.label44.Location = new System.Drawing.Point(169, 31);
            this.label44.Name = "label44";
            this.label44.Size = new System.Drawing.Size(56, 16);
            this.label44.TabIndex = 176;
            this.label44.Text = "File Name";
            // 
            // label43
            // 
            this.label43.Location = new System.Drawing.Point(129, 31);
            this.label43.Name = "label43";
            this.label43.Size = new System.Drawing.Size(16, 16);
            this.label43.TabIndex = 167;
            this.label43.Text = "Y";
            // 
            // label41
            // 
            this.label41.Location = new System.Drawing.Point(81, 31);
            this.label41.Name = "label41";
            this.label41.Size = new System.Drawing.Size(16, 16);
            this.label41.TabIndex = 165;
            this.label41.Text = "X";
            // 
            // inbox2
            // 
            this.inbox2.Location = new System.Drawing.Point(9, 79);
            this.inbox2.Name = "inbox2";
            this.inbox2.Size = new System.Drawing.Size(64, 16);
            this.inbox2.TabIndex = 164;
            this.inbox2.Text = "Input 2";
            // 
            // inbox3
            // 
            this.inbox3.Location = new System.Drawing.Point(9, 103);
            this.inbox3.Name = "inbox3";
            this.inbox3.Size = new System.Drawing.Size(64, 16);
            this.inbox3.TabIndex = 163;
            this.inbox3.Text = "Input 3";
            // 
            // inbox4
            // 
            this.inbox4.Location = new System.Drawing.Point(9, 127);
            this.inbox4.Name = "inbox4";
            this.inbox4.Size = new System.Drawing.Size(64, 16);
            this.inbox4.TabIndex = 162;
            this.inbox4.Text = "Input 4";
            // 
            // inbox1
            // 
            this.inbox1.Location = new System.Drawing.Point(9, 55);
            this.inbox1.Name = "inbox1";
            this.inbox1.Size = new System.Drawing.Size(64, 16);
            this.inbox1.TabIndex = 161;
            this.inbox1.Text = "Input 1";
            // 
            // label42
            // 
            this.label42.Location = new System.Drawing.Point(81, 31);
            this.label42.Name = "label42";
            this.label42.Size = new System.Drawing.Size(16, 16);
            this.label42.TabIndex = 166;
            this.label42.Text = "X";
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.label90);
            this.groupBox1.Controls.Add(this.TidalFileName);
            this.groupBox1.Controls.Add(this.TidalInputStep);
            this.groupBox1.Controls.Add(this.label82);
            this.groupBox1.Controls.Add(this.TidalYmin);
            this.groupBox1.Controls.Add(this.TidalYmax);
            this.groupBox1.Controls.Add(this.TidalXmax);
            this.groupBox1.Controls.Add(this.TidalXmin);
            this.groupBox1.Controls.Add(this.label80);
            this.groupBox1.Controls.Add(this.label81);
            this.groupBox1.Location = new System.Drawing.Point(526, 303);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(364, 202);
            this.groupBox1.TabIndex = 213;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Stage/Tidal input variables";
            // 
            // label90
            // 
            this.label90.Location = new System.Drawing.Point(206, 45);
            this.label90.Name = "label90";
            this.label90.Size = new System.Drawing.Size(55, 15);
            this.label90.TabIndex = 212;
            this.label90.Text = "File Name";
            // 
            // TidalFileName
            // 
            this.TidalFileName.Location = new System.Drawing.Point(207, 62);
            this.TidalFileName.Name = "TidalFileName";
            this.TidalFileName.Size = new System.Drawing.Size(120, 20);
            this.TidalFileName.TabIndex = 211;
            // 
            // TidalInputStep
            // 
            this.TidalInputStep.Location = new System.Drawing.Point(52, 121);
            this.TidalInputStep.Name = "TidalInputStep";
            this.TidalInputStep.Size = new System.Drawing.Size(63, 20);
            this.TidalInputStep.TabIndex = 209;
            this.TidalInputStep.Text = "1440";
            // 
            // label82
            // 
            this.label82.Location = new System.Drawing.Point(125, 114);
            this.label82.Name = "label82";
            this.label82.Size = new System.Drawing.Size(100, 32);
            this.label82.TabIndex = 210;
            this.label82.Text = "input data time step (mins)";
            // 
            // TidalYmin
            // 
            this.TidalYmin.Location = new System.Drawing.Point(93, 39);
            this.TidalYmin.Name = "TidalYmin";
            this.TidalYmin.Size = new System.Drawing.Size(32, 20);
            this.TidalYmin.TabIndex = 208;
            this.TidalYmin.Text = "0";
            // 
            // TidalYmax
            // 
            this.TidalYmax.Location = new System.Drawing.Point(93, 87);
            this.TidalYmax.Name = "TidalYmax";
            this.TidalYmax.Size = new System.Drawing.Size(32, 20);
            this.TidalYmax.TabIndex = 207;
            this.TidalYmax.Text = "0";
            // 
            // TidalXmax
            // 
            this.TidalXmax.Location = new System.Drawing.Point(128, 62);
            this.TidalXmax.Name = "TidalXmax";
            this.TidalXmax.Size = new System.Drawing.Size(31, 20);
            this.TidalXmax.TabIndex = 206;
            this.TidalXmax.Text = "0";
            // 
            // TidalXmin
            // 
            this.TidalXmin.Location = new System.Drawing.Point(54, 62);
            this.TidalXmin.Name = "TidalXmin";
            this.TidalXmin.Size = new System.Drawing.Size(31, 20);
            this.TidalXmin.TabIndex = 205;
            this.TidalXmin.Text = "0";
            // 
            // label80
            // 
            this.label80.Location = new System.Drawing.Point(100, 21);
            this.label80.Name = "label80";
            this.label80.Size = new System.Drawing.Size(15, 15);
            this.label80.TabIndex = 204;
            this.label80.Text = "Y";
            // 
            // label81
            // 
            this.label81.Location = new System.Drawing.Point(32, 65);
            this.label81.Name = "label81";
            this.label81.Size = new System.Drawing.Size(16, 15);
            this.label81.TabIndex = 203;
            this.label81.Text = "X";
            // 
            // tabPage2
            // 
            this.tabPage2.Controls.Add(this.groupBox8);
            this.tabPage2.Controls.Add(this.veg_lat_box);
            this.tabPage2.Controls.Add(this.label51);
            this.tabPage2.Controls.Add(this.grasstextbox);
            this.tabPage2.Controls.Add(this.label40);
            this.tabPage2.Controls.Add(this.label36);
            this.tabPage2.Controls.Add(this.vegTauCritBox);
            this.tabPage2.Location = new System.Drawing.Point(4, 22);
            this.tabPage2.Name = "tabPage2";
            this.tabPage2.Size = new System.Drawing.Size(1323, 487);
            this.tabPage2.TabIndex = 7;
            this.tabPage2.Text = "Vegetation";
            this.tabPage2.UseVisualStyleBackColor = true;
            // 
            // groupBox8
            // 
            this.groupBox8.Controls.Add(this.radioButton2);
            this.groupBox8.Controls.Add(this.radioButton1);
            this.groupBox8.Location = new System.Drawing.Point(284, 32);
            this.groupBox8.Name = "groupBox8";
            this.groupBox8.Size = new System.Drawing.Size(117, 77);
            this.groupBox8.TabIndex = 178;
            this.groupBox8.TabStop = false;
            this.groupBox8.Text = "Vegetation model";
            // 
            // radioButton2
            // 
            this.radioButton2.AutoSize = true;
            this.radioButton2.Location = new System.Drawing.Point(17, 43);
            this.radioButton2.Name = "radioButton2";
            this.radioButton2.Size = new System.Drawing.Size(84, 17);
            this.radioButton2.TabIndex = 177;
            this.radioButton2.Text = "new (>=1.9g)";
            this.radioButton2.UseVisualStyleBackColor = true;
            // 
            // radioButton1
            // 
            this.radioButton1.AutoSize = true;
            this.radioButton1.Checked = true;
            this.radioButton1.Location = new System.Drawing.Point(17, 20);
            this.radioButton1.Name = "radioButton1";
            this.radioButton1.Size = new System.Drawing.Size(75, 17);
            this.radioButton1.TabIndex = 176;
            this.radioButton1.TabStop = true;
            this.radioButton1.Text = "old (<1.9d)";
            this.radioButton1.UseVisualStyleBackColor = true;
            // 
            // veg_lat_box
            // 
            this.veg_lat_box.Location = new System.Drawing.Point(157, 117);
            this.veg_lat_box.Name = "veg_lat_box";
            this.veg_lat_box.Size = new System.Drawing.Size(40, 20);
            this.veg_lat_box.TabIndex = 175;
            this.veg_lat_box.Text = "0.1";
            // 
            // label51
            // 
            this.label51.Location = new System.Drawing.Point(29, 103);
            this.label51.Name = "label51";
            this.label51.Size = new System.Drawing.Size(120, 47);
            this.label51.TabIndex = 174;
            this.label51.Text = "Proportion of erosion that can occur when veg is fully grown (0-1)";
            this.label51.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // grasstextbox
            // 
            this.grasstextbox.Location = new System.Drawing.Point(157, 68);
            this.grasstextbox.Name = "grasstextbox";
            this.grasstextbox.Size = new System.Drawing.Size(40, 20);
            this.grasstextbox.TabIndex = 172;
            this.grasstextbox.Text = "5";
            // 
            // label40
            // 
            this.label40.Location = new System.Drawing.Point(29, 68);
            this.label40.Name = "label40";
            this.label40.Size = new System.Drawing.Size(120, 24);
            this.label40.TabIndex = 173;
            this.label40.Text = "Grass maturity (yrs)";
            this.label40.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // label36
            // 
            this.label36.Location = new System.Drawing.Point(37, 28);
            this.label36.Name = "label36";
            this.label36.Size = new System.Drawing.Size(112, 24);
            this.label36.TabIndex = 108;
            this.label36.Text = "vegetation crit shear";
            this.label36.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // vegTauCritBox
            // 
            this.vegTauCritBox.Location = new System.Drawing.Point(157, 32);
            this.vegTauCritBox.Name = "vegTauCritBox";
            this.vegTauCritBox.Size = new System.Drawing.Size(71, 20);
            this.vegTauCritBox.TabIndex = 92;
            this.vegTauCritBox.Text = "180.0";
            // 
            // tabPage4
            // 
            this.tabPage4.Controls.Add(this.soilerosionBox);
            this.tabPage4.Controls.Add(this.landslidesBox);
            this.tabPage4.Controls.Add(this.label75);
            this.tabPage4.Controls.Add(this.label74);
            this.tabPage4.Controls.Add(this.label73);
            this.tabPage4.Controls.Add(this.label72);
            this.tabPage4.Controls.Add(this.label71);
            this.tabPage4.Controls.Add(this.m3Box);
            this.tabPage4.Controls.Add(this.n1Box);
            this.tabPage4.Controls.Add(this.m1Box);
            this.tabPage4.Controls.Add(this.Beta3Box);
            this.tabPage4.Controls.Add(this.Beta1Box);
            this.tabPage4.Controls.Add(this.SiberiaBox);
            this.tabPage4.Controls.Add(this.label70);
            this.tabPage4.Controls.Add(this.label69);
            this.tabPage4.Controls.Add(this.label68);
            this.tabPage4.Controls.Add(this.label67);
            this.tabPage4.Controls.Add(this.soil_ratebox);
            this.tabPage4.Controls.Add(this.slopebox);
            this.tabPage4.Controls.Add(this.creepratebox);
            this.tabPage4.Controls.Add(this.label34);
            this.tabPage4.Controls.Add(this.label8);
            this.tabPage4.Location = new System.Drawing.Point(4, 22);
            this.tabPage4.Name = "tabPage4";
            this.tabPage4.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage4.Size = new System.Drawing.Size(1323, 487);
            this.tabPage4.TabIndex = 9;
            this.tabPage4.Text = "Slope Processes";
            this.tabPage4.UseVisualStyleBackColor = true;
            // 
            // soilerosionBox
            // 
            this.soilerosionBox.Location = new System.Drawing.Point(223, 163);
            this.soilerosionBox.Name = "soilerosionBox";
            this.soilerosionBox.Size = new System.Drawing.Size(147, 34);
            this.soilerosionBox.TabIndex = 187;
            this.soilerosionBox.Text = "Soil erosion varies according to j_mean";
            this.toolTip1.SetToolTip(this.soilerosionBox, "CAESAR can run in both catchment and reach mode, but if you have catchment mode c" +
        "hecked, you should input a rainfall data file");
            // 
            // landslidesBox
            // 
            this.landslidesBox.Location = new System.Drawing.Point(223, 69);
            this.landslidesBox.Name = "landslidesBox";
            this.landslidesBox.Size = new System.Drawing.Size(196, 34);
            this.landslidesBox.TabIndex = 186;
            this.landslidesBox.Text = "Dynamic Slope fail angle -  varies according to j_mean";
            // 
            // label75
            // 
            this.label75.Location = new System.Drawing.Point(648, 174);
            this.label75.Name = "label75";
            this.label75.Size = new System.Drawing.Size(96, 24);
            this.label75.TabIndex = 185;
            this.label75.Text = "n1";
            this.label75.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // label74
            // 
            this.label74.Location = new System.Drawing.Point(648, 151);
            this.label74.Name = "label74";
            this.label74.Size = new System.Drawing.Size(96, 24);
            this.label74.TabIndex = 184;
            this.label74.Text = "m3";
            this.label74.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // label73
            // 
            this.label73.Location = new System.Drawing.Point(648, 73);
            this.label73.Name = "label73";
            this.label73.Size = new System.Drawing.Size(96, 24);
            this.label73.TabIndex = 183;
            this.label73.Text = "Beta1";
            this.label73.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // label72
            // 
            this.label72.Location = new System.Drawing.Point(648, 125);
            this.label72.Name = "label72";
            this.label72.Size = new System.Drawing.Size(96, 24);
            this.label72.TabIndex = 182;
            this.label72.Text = "m1";
            this.label72.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // label71
            // 
            this.label71.Location = new System.Drawing.Point(648, 99);
            this.label71.Name = "label71";
            this.label71.Size = new System.Drawing.Size(96, 24);
            this.label71.TabIndex = 181;
            this.label71.Text = "Beta3";
            this.label71.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // m3Box
            // 
            this.m3Box.Location = new System.Drawing.Point(750, 151);
            this.m3Box.Name = "m3Box";
            this.m3Box.Size = new System.Drawing.Size(49, 20);
            this.m3Box.TabIndex = 180;
            this.m3Box.Text = "0.79";
            // 
            // n1Box
            // 
            this.n1Box.Location = new System.Drawing.Point(750, 177);
            this.n1Box.Name = "n1Box";
            this.n1Box.Size = new System.Drawing.Size(49, 20);
            this.n1Box.TabIndex = 179;
            this.n1Box.Text = "0.69";
            this.n1Box.TextChanged += new System.EventHandler(this.n1Box_TextChanged);
            // 
            // m1Box
            // 
            this.m1Box.Location = new System.Drawing.Point(750, 125);
            this.m1Box.Name = "m1Box";
            this.m1Box.Size = new System.Drawing.Size(49, 20);
            this.m1Box.TabIndex = 178;
            this.m1Box.Text = "1.7";
            // 
            // Beta3Box
            // 
            this.Beta3Box.Location = new System.Drawing.Point(750, 99);
            this.Beta3Box.Name = "Beta3Box";
            this.Beta3Box.Size = new System.Drawing.Size(49, 20);
            this.Beta3Box.TabIndex = 177;
            this.Beta3Box.Text = "0.000186";
            // 
            // Beta1Box
            // 
            this.Beta1Box.Location = new System.Drawing.Point(750, 73);
            this.Beta1Box.Name = "Beta1Box";
            this.Beta1Box.Size = new System.Drawing.Size(49, 20);
            this.Beta1Box.TabIndex = 176;
            this.Beta1Box.Text = "1067";
            // 
            // SiberiaBox
            // 
            this.SiberiaBox.Location = new System.Drawing.Point(692, 36);
            this.SiberiaBox.Name = "SiberiaBox";
            this.SiberiaBox.Size = new System.Drawing.Size(147, 34);
            this.SiberiaBox.TabIndex = 175;
            this.SiberiaBox.Text = "SIBERIA sub model?";
            this.toolTip1.SetToolTip(this.SiberiaBox, "CAESAR can run in both catchment and reach mode, but if you have catchment mode c" +
        "hecked, you should input a rainfall data file");
            // 
            // label70
            // 
            this.label70.AutoSize = true;
            this.label70.Location = new System.Drawing.Point(220, 132);
            this.label70.Name = "label70";
            this.label70.Size = new System.Drawing.Size(246, 13);
            this.label70.TabIndex = 168;
            this.label70.Text = "simply slope * slope length, so replicates wash term";
            // 
            // label69
            // 
            this.label69.AutoSize = true;
            this.label69.Location = new System.Drawing.Point(220, 116);
            this.label69.Name = "label69";
            this.label69.Size = new System.Drawing.Size(357, 13);
            this.label69.TabIndex = 167;
            this.label69.Text = "Slope * Soil erosion rate * (drainage area ^ 0.5) * Time(years) / DX(cellsize)";
            // 
            // label68
            // 
            this.label68.AutoSize = true;
            this.label68.Location = new System.Drawing.Point(220, 42);
            this.label68.Name = "label68";
            this.label68.Size = new System.Drawing.Size(220, 13);
            this.label68.TabIndex = 166;
            this.label68.Text = "Slope * Creeprate * Time(years) / DX(cellsize)";
            // 
            // label67
            // 
            this.label67.Location = new System.Drawing.Point(51, 109);
            this.label67.Name = "label67";
            this.label67.Size = new System.Drawing.Size(96, 24);
            this.label67.TabIndex = 165;
            this.label67.Text = "Soil erosion rate";
            this.label67.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // soil_ratebox
            // 
            this.soil_ratebox.Location = new System.Drawing.Point(155, 113);
            this.soil_ratebox.Name = "soil_ratebox";
            this.soil_ratebox.Size = new System.Drawing.Size(49, 20);
            this.soil_ratebox.TabIndex = 164;
            this.soil_ratebox.Text = "0.0";
            // 
            // slopebox
            // 
            this.slopebox.Location = new System.Drawing.Point(155, 75);
            this.slopebox.Name = "slopebox";
            this.slopebox.Size = new System.Drawing.Size(40, 20);
            this.slopebox.TabIndex = 162;
            this.slopebox.Text = "45";
            // 
            // creepratebox
            // 
            this.creepratebox.Location = new System.Drawing.Point(155, 39);
            this.creepratebox.Name = "creepratebox";
            this.creepratebox.Size = new System.Drawing.Size(49, 20);
            this.creepratebox.TabIndex = 160;
            this.creepratebox.Text = "0.0025";
            // 
            // label34
            // 
            this.label34.Location = new System.Drawing.Point(21, 75);
            this.label34.Name = "label34";
            this.label34.Size = new System.Drawing.Size(128, 24);
            this.label34.TabIndex = 163;
            this.label34.Text = "slope failure threshold";
            this.label34.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // label8
            // 
            this.label8.Location = new System.Drawing.Point(51, 39);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(96, 24);
            this.label8.TabIndex = 161;
            this.label8.Text = "Creep rate";
            this.label8.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // tabPage5
            // 
            this.tabPage5.Controls.Add(this.label91);
            this.tabPage5.Controls.Add(this.textBox10);
            this.tabPage5.Controls.Add(this.fraction_dune);
            this.tabPage5.Controls.Add(this.label57);
            this.tabPage5.Controls.Add(this.dune_grid_size_box);
            this.tabPage5.Controls.Add(this.label56);
            this.tabPage5.Controls.Add(this.dune_time_box);
            this.tabPage5.Controls.Add(this.label89);
            this.tabPage5.Controls.Add(this.label88);
            this.tabPage5.Controls.Add(this.label87);
            this.tabPage5.Controls.Add(this.label86);
            this.tabPage5.Controls.Add(this.label85);
            this.tabPage5.Controls.Add(this.label84);
            this.tabPage5.Controls.Add(this.slab_depth_box);
            this.tabPage5.Controls.Add(this.shadow_angle_box);
            this.tabPage5.Controls.Add(this.upstream_check_box);
            this.tabPage5.Controls.Add(this.depo_prob_box);
            this.tabPage5.Controls.Add(this.offset_box);
            this.tabPage5.Controls.Add(this.init_depth_box);
            this.tabPage5.Controls.Add(this.DuneBox);
            this.tabPage5.Location = new System.Drawing.Point(4, 22);
            this.tabPage5.Name = "tabPage5";
            this.tabPage5.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage5.Size = new System.Drawing.Size(1323, 487);
            this.tabPage5.TabIndex = 10;
            this.tabPage5.Text = "Dune model";
            this.tabPage5.UseVisualStyleBackColor = true;
            this.tabPage5.Click += new System.EventHandler(this.tabPage5_Click);
            // 
            // label91
            // 
            this.label91.AutoSize = true;
            this.label91.Location = new System.Drawing.Point(115, 153);
            this.label91.Name = "label91";
            this.label91.Size = new System.Drawing.Size(100, 13);
            this.label91.TabIndex = 19;
            this.label91.Text = "Dune landslip angle";
            // 
            // textBox10
            // 
            this.textBox10.Location = new System.Drawing.Point(26, 150);
            this.textBox10.Name = "textBox10";
            this.textBox10.Size = new System.Drawing.Size(75, 20);
            this.textBox10.TabIndex = 18;
            this.textBox10.Text = "30";
            // 
            // fraction_dune
            // 
            this.fraction_dune.Location = new System.Drawing.Point(512, 113);
            this.fraction_dune.Name = "fraction_dune";
            this.fraction_dune.Size = new System.Drawing.Size(75, 20);
            this.fraction_dune.TabIndex = 17;
            this.fraction_dune.Text = "1";
            // 
            // label57
            // 
            this.label57.AutoSize = true;
            this.label57.Location = new System.Drawing.Point(600, 79);
            this.label57.Name = "label57";
            this.label57.Size = new System.Drawing.Size(91, 13);
            this.label57.TabIndex = 16;
            this.label57.Text = "Grid size of dunes";
            // 
            // dune_grid_size_box
            // 
            this.dune_grid_size_box.Location = new System.Drawing.Point(512, 76);
            this.dune_grid_size_box.Name = "dune_grid_size_box";
            this.dune_grid_size_box.Size = new System.Drawing.Size(75, 20);
            this.dune_grid_size_box.TabIndex = 15;
            this.dune_grid_size_box.Text = "2";
            // 
            // label56
            // 
            this.label56.AutoSize = true;
            this.label56.Location = new System.Drawing.Point(600, 44);
            this.label56.Name = "label56";
            this.label56.Size = new System.Drawing.Size(169, 13);
            this.label56.TabIndex = 14;
            this.label56.Text = "time step (min) between dune calls";
            // 
            // dune_time_box
            // 
            this.dune_time_box.Location = new System.Drawing.Point(512, 41);
            this.dune_time_box.Name = "dune_time_box";
            this.dune_time_box.Size = new System.Drawing.Size(75, 20);
            this.dune_time_box.TabIndex = 13;
            this.dune_time_box.Text = "144";
            // 
            // label89
            // 
            this.label89.AutoSize = true;
            this.label89.Location = new System.Drawing.Point(117, 257);
            this.label89.Name = "label89";
            this.label89.Size = new System.Drawing.Size(270, 13);
            this.label89.TabIndex = 12;
            this.label89.Text = "Downstream offset (travel distance automatically added)";
            // 
            // label88
            // 
            this.label88.AutoSize = true;
            this.label88.Location = new System.Drawing.Point(115, 222);
            this.label88.Name = "label88";
            this.label88.Size = new System.Drawing.Size(124, 13);
            this.label88.TabIndex = 11;
            this.label88.Text = "Deposition probability (%)";
            this.label88.Click += new System.EventHandler(this.label88_Click);
            // 
            // label87
            // 
            this.label87.AutoSize = true;
            this.label87.Location = new System.Drawing.Point(115, 186);
            this.label87.Name = "label87";
            this.label87.Size = new System.Drawing.Size(381, 13);
            this.label87.TabIndex = 10;
            this.label87.Text = "Shadow check distance (cells) - number of cells Upstream it checks for shadow";
            // 
            // label86
            // 
            this.label86.AutoSize = true;
            this.label86.Location = new System.Drawing.Point(115, 118);
            this.label86.Name = "label86";
            this.label86.Size = new System.Drawing.Size(105, 13);
            this.label86.TabIndex = 9;
            this.label86.Text = "Shadow angle (deg) ";
            // 
            // label85
            // 
            this.label85.AutoSize = true;
            this.label85.Location = new System.Drawing.Point(117, 83);
            this.label85.Name = "label85";
            this.label85.Size = new System.Drawing.Size(138, 13);
            this.label85.TabIndex = 8;
            this.label85.Text = "Maximum slab thickness (m)";
            // 
            // label84
            // 
            this.label84.AutoSize = true;
            this.label84.Location = new System.Drawing.Point(115, 47);
            this.label84.Name = "label84";
            this.label84.Size = new System.Drawing.Size(185, 13);
            this.label84.TabIndex = 7;
            this.label84.Text = "how many slabs added per col per iter";
            // 
            // slab_depth_box
            // 
            this.slab_depth_box.Location = new System.Drawing.Point(26, 80);
            this.slab_depth_box.Name = "slab_depth_box";
            this.slab_depth_box.Size = new System.Drawing.Size(75, 20);
            this.slab_depth_box.TabIndex = 6;
            this.slab_depth_box.Text = "0.5";
            this.slab_depth_box.TextChanged += new System.EventHandler(this.textBox12_TextChanged);
            // 
            // shadow_angle_box
            // 
            this.shadow_angle_box.Location = new System.Drawing.Point(26, 115);
            this.shadow_angle_box.Name = "shadow_angle_box";
            this.shadow_angle_box.Size = new System.Drawing.Size(75, 20);
            this.shadow_angle_box.TabIndex = 5;
            this.shadow_angle_box.Text = "15";
            // 
            // upstream_check_box
            // 
            this.upstream_check_box.Location = new System.Drawing.Point(26, 183);
            this.upstream_check_box.Name = "upstream_check_box";
            this.upstream_check_box.Size = new System.Drawing.Size(75, 20);
            this.upstream_check_box.TabIndex = 4;
            this.upstream_check_box.Text = "40";
            // 
            // depo_prob_box
            // 
            this.depo_prob_box.Location = new System.Drawing.Point(27, 219);
            this.depo_prob_box.Name = "depo_prob_box";
            this.depo_prob_box.Size = new System.Drawing.Size(75, 20);
            this.depo_prob_box.TabIndex = 3;
            this.depo_prob_box.Text = "50";
            // 
            // offset_box
            // 
            this.offset_box.Location = new System.Drawing.Point(26, 254);
            this.offset_box.Name = "offset_box";
            this.offset_box.Size = new System.Drawing.Size(75, 20);
            this.offset_box.TabIndex = 2;
            this.offset_box.Text = "1";
            // 
            // init_depth_box
            // 
            this.init_depth_box.Location = new System.Drawing.Point(27, 44);
            this.init_depth_box.Name = "init_depth_box";
            this.init_depth_box.Size = new System.Drawing.Size(75, 20);
            this.init_depth_box.TabIndex = 1;
            this.init_depth_box.Text = "4";
            // 
            // DuneBox
            // 
            this.DuneBox.AutoSize = true;
            this.DuneBox.Location = new System.Drawing.Point(26, 18);
            this.DuneBox.Name = "DuneBox";
            this.DuneBox.Size = new System.Drawing.Size(108, 17);
            this.DuneBox.TabIndex = 0;
            this.DuneBox.Text = "Run with Dunes?";
            this.DuneBox.UseVisualStyleBackColor = true;
            // 
            // tabPage1
            // 
            this.tabPage1.Controls.Add(this.textBox19);
            this.tabPage1.Controls.Add(this.label104);
            this.tabPage1.Controls.Add(this.SpatVarManningsCheckbox);
            this.tabPage1.Controls.Add(this.MinQmaxvalue);
            this.tabPage1.Controls.Add(this.textBox9);
            this.tabPage1.Controls.Add(this.label77);
            this.tabPage1.Controls.Add(this.textBox8);
            this.tabPage1.Controls.Add(this.label66);
            this.tabPage1.Controls.Add(this.textBox4);
            this.tabPage1.Controls.Add(this.label64);
            this.tabPage1.Controls.Add(this.courantbox);
            this.tabPage1.Controls.Add(this.label38);
            this.tabPage1.Controls.Add(this.label53);
            this.tabPage1.Controls.Add(this.Q2box);
            this.tabPage1.Controls.Add(this.label3);
            this.tabPage1.Controls.Add(this.k_evapBox);
            this.tabPage1.Controls.Add(this.textBox2);
            this.tabPage1.Controls.Add(this.label46);
            this.tabPage1.Controls.Add(this.minqbox);
            this.tabPage1.Controls.Add(this.initscansbox);
            this.tabPage1.Controls.Add(this.label9);
            this.tabPage1.Controls.Add(this.label5);
            this.tabPage1.Location = new System.Drawing.Point(4, 22);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage1.Size = new System.Drawing.Size(1323, 487);
            this.tabPage1.TabIndex = 11;
            this.tabPage1.Text = "Flow Model";
            this.tabPage1.UseVisualStyleBackColor = true;
            // 
            // textBox19
            // 
            this.textBox19.Location = new System.Drawing.Point(331, 341);
            this.textBox19.Name = "textBox19";
            this.textBox19.Size = new System.Drawing.Size(118, 20);
            this.textBox19.TabIndex = 235;
            this.textBox19.Text = "null";
            this.textBox19.Visible = false;
            // 
            // label104
            // 
            this.label104.AutoSize = true;
            this.label104.Location = new System.Drawing.Point(247, 344);
            this.label104.Name = "label104";
            this.label104.Size = new System.Drawing.Size(78, 13);
            this.label104.TabIndex = 234;
            this.label104.Text = "Mannings n file";
            this.label104.Visible = false;
            // 
            // SpatVarManningsCheckbox
            // 
            this.SpatVarManningsCheckbox.AutoSize = true;
            this.SpatVarManningsCheckbox.Location = new System.Drawing.Point(293, 318);
            this.SpatVarManningsCheckbox.Name = "SpatVarManningsCheckbox";
            this.SpatVarManningsCheckbox.Size = new System.Drawing.Size(162, 17);
            this.SpatVarManningsCheckbox.TabIndex = 230;
            this.SpatVarManningsCheckbox.Text = "Spatially variable mannings n";
            this.SpatVarManningsCheckbox.UseVisualStyleBackColor = true;
            this.SpatVarManningsCheckbox.CheckedChanged += new System.EventHandler(this.SpatVarManningsCheckbox_CheckedChanged);
            // 
            // MinQmaxvalue
            // 
            this.MinQmaxvalue.Location = new System.Drawing.Point(302, 106);
            this.MinQmaxvalue.Name = "MinQmaxvalue";
            this.MinQmaxvalue.Size = new System.Drawing.Size(49, 20);
            this.MinQmaxvalue.TabIndex = 216;
            this.MinQmaxvalue.Text = "1000.0";
            // 
            // textBox9
            // 
            this.textBox9.Location = new System.Drawing.Point(229, 315);
            this.textBox9.Name = "textBox9";
            this.textBox9.Size = new System.Drawing.Size(38, 20);
            this.textBox9.TabIndex = 215;
            this.textBox9.Text = "0.04";
            // 
            // label77
            // 
            this.label77.Location = new System.Drawing.Point(95, 311);
            this.label77.Name = "label77";
            this.label77.Size = new System.Drawing.Size(128, 24);
            this.label77.TabIndex = 214;
            this.label77.Text = "Mannings n";
            this.label77.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // textBox8
            // 
            this.textBox8.Location = new System.Drawing.Point(229, 291);
            this.textBox8.Name = "textBox8";
            this.textBox8.Size = new System.Drawing.Size(38, 20);
            this.textBox8.TabIndex = 213;
            this.textBox8.Text = "0.8";
            // 
            // label66
            // 
            this.label66.Location = new System.Drawing.Point(95, 287);
            this.label66.Name = "label66";
            this.label66.Size = new System.Drawing.Size(128, 24);
            this.label66.TabIndex = 212;
            this.label66.Text = "froude # flow limit";
            this.label66.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // textBox4
            // 
            this.textBox4.Location = new System.Drawing.Point(229, 267);
            this.textBox4.Name = "textBox4";
            this.textBox4.Size = new System.Drawing.Size(38, 20);
            this.textBox4.TabIndex = 211;
            this.textBox4.Text = "0.00001";
            // 
            // label64
            // 
            this.label64.Location = new System.Drawing.Point(95, 263);
            this.label64.Name = "label64";
            this.label64.Size = new System.Drawing.Size(128, 24);
            this.label64.TabIndex = 210;
            this.label64.Text = "hflow threshold";
            this.label64.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // courantbox
            // 
            this.courantbox.Location = new System.Drawing.Point(229, 241);
            this.courantbox.Name = "courantbox";
            this.courantbox.Size = new System.Drawing.Size(38, 20);
            this.courantbox.TabIndex = 209;
            this.courantbox.Text = "0.7";
            // 
            // label38
            // 
            this.label38.Location = new System.Drawing.Point(95, 237);
            this.label38.Name = "label38";
            this.label38.Size = new System.Drawing.Size(128, 24);
            this.label38.TabIndex = 208;
            this.label38.Text = "Courant Number";
            this.label38.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // label53
            // 
            this.label53.Location = new System.Drawing.Point(101, 130);
            this.label53.Name = "label53";
            this.label53.Size = new System.Drawing.Size(120, 43);
            this.label53.TabIndex = 205;
            this.label53.Text = "Water depth threshold above which erosion will happen (m)";
            this.label53.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.toolTip1.SetToolTip(this.label53, "MinQ is a threshold value, above which CAEASR treats flow as surface flow, it sho" +
        "uld be scaled to the grid size. Try 0.01 for 10m grids and 0.25 for 50m..");
            // 
            // Q2box
            // 
            this.Q2box.Location = new System.Drawing.Point(229, 137);
            this.Q2box.Name = "Q2box";
            this.Q2box.Size = new System.Drawing.Size(49, 20);
            this.Q2box.TabIndex = 204;
            this.Q2box.Text = "0.01";
            // 
            // label3
            // 
            this.label3.Location = new System.Drawing.Point(82, 212);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(139, 24);
            this.label3.TabIndex = 203;
            this.label3.Text = "Evaporation rate (m/day)";
            this.label3.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // k_evapBox
            // 
            this.k_evapBox.Location = new System.Drawing.Point(229, 215);
            this.k_evapBox.Name = "k_evapBox";
            this.k_evapBox.Size = new System.Drawing.Size(80, 20);
            this.k_evapBox.TabIndex = 202;
            this.k_evapBox.Text = "0.0";
            // 
            // textBox2
            // 
            this.textBox2.Location = new System.Drawing.Point(229, 182);
            this.textBox2.Name = "textBox2";
            this.textBox2.Size = new System.Drawing.Size(40, 20);
            this.textBox2.TabIndex = 200;
            this.textBox2.Text = "0.005";
            // 
            // label46
            // 
            this.label46.Location = new System.Drawing.Point(93, 179);
            this.label46.Name = "label46";
            this.label46.Size = new System.Drawing.Size(128, 24);
            this.label46.TabIndex = 201;
            this.label46.Text = "Slope for edge cells";
            this.label46.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // minqbox
            // 
            this.minqbox.Location = new System.Drawing.Point(229, 106);
            this.minqbox.Name = "minqbox";
            this.minqbox.Size = new System.Drawing.Size(49, 20);
            this.minqbox.TabIndex = 197;
            this.minqbox.Text = "0.01";
            // 
            // initscansbox
            // 
            this.initscansbox.Location = new System.Drawing.Point(227, 68);
            this.initscansbox.Name = "initscansbox";
            this.initscansbox.Size = new System.Drawing.Size(40, 20);
            this.initscansbox.TabIndex = 196;
            this.initscansbox.Text = "1";
            // 
            // label9
            // 
            this.label9.Location = new System.Drawing.Point(101, 106);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(120, 24);
            this.label9.TabIndex = 199;
            this.label9.Text = "Min Q for depth calc";
            this.label9.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.toolTip1.SetToolTip(this.label9, "MinQ is a threshold value, above which CAEASR treats flow as surface flow, it sho" +
        "uld be scaled to the grid size. Try 0.01 for 10m grids and 0.25 for 50m..");
            // 
            // label5
            // 
            this.label5.Location = new System.Drawing.Point(29, 65);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(182, 26);
            this.label5.TabIndex = 198;
            this.label5.Text = "input/output difference allowed";
            this.label5.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // tabPage3
            // 
            this.tabPage3.Controls.Add(this.label101);
            this.tabPage3.Controls.Add(this.label100);
            this.tabPage3.Controls.Add(this.label99);
            this.tabPage3.Controls.Add(this.textBox18);
            this.tabPage3.Controls.Add(this.textBox17);
            this.tabPage3.Controls.Add(this.textBox16);
            this.tabPage3.Controls.Add(this.label98);
            this.tabPage3.Controls.Add(this.label97);
            this.tabPage3.Controls.Add(this.label96);
            this.tabPage3.Controls.Add(this.textBox15);
            this.tabPage3.Controls.Add(this.textBox14);
            this.tabPage3.Controls.Add(this.textBox13);
            this.tabPage3.Controls.Add(this.label95);
            this.tabPage3.Controls.Add(this.label94);
            this.tabPage3.Controls.Add(this.textBox12);
            this.tabPage3.Controls.Add(this.textBox11);
            this.tabPage3.Controls.Add(this.checkBox6);
            this.tabPage3.Controls.Add(this.checkBox5);
            this.tabPage3.Controls.Add(this.checkBox4);
            this.tabPage3.Controls.Add(this.soildevbox);
            this.tabPage3.Location = new System.Drawing.Point(4, 22);
            this.tabPage3.Name = "tabPage3";
            this.tabPage3.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage3.Size = new System.Drawing.Size(1323, 487);
            this.tabPage3.TabIndex = 12;
            this.tabPage3.Text = "Soil Development";
            this.tabPage3.UseVisualStyleBackColor = true;
            // 
            // label101
            // 
            this.label101.AutoSize = true;
            this.label101.Enabled = false;
            this.label101.Location = new System.Drawing.Point(414, 153);
            this.label101.Name = "label101";
            this.label101.Size = new System.Drawing.Size(19, 13);
            this.label101.TabIndex = 19;
            this.label101.Text = "c4";
            // 
            // label100
            // 
            this.label100.AutoSize = true;
            this.label100.Enabled = false;
            this.label100.Location = new System.Drawing.Point(414, 124);
            this.label100.Name = "label100";
            this.label100.Size = new System.Drawing.Size(19, 13);
            this.label100.TabIndex = 18;
            this.label100.Text = "c3";
            // 
            // label99
            // 
            this.label99.AutoSize = true;
            this.label99.Enabled = false;
            this.label99.Location = new System.Drawing.Point(415, 94);
            this.label99.Name = "label99";
            this.label99.Size = new System.Drawing.Size(19, 13);
            this.label99.TabIndex = 17;
            this.label99.Text = "k2";
            // 
            // textBox18
            // 
            this.textBox18.Enabled = false;
            this.textBox18.Location = new System.Drawing.Point(336, 150);
            this.textBox18.Name = "textBox18";
            this.textBox18.Size = new System.Drawing.Size(57, 20);
            this.textBox18.TabIndex = 16;
            this.textBox18.Text = "1";
            // 
            // textBox17
            // 
            this.textBox17.Enabled = false;
            this.textBox17.Location = new System.Drawing.Point(336, 121);
            this.textBox17.Name = "textBox17";
            this.textBox17.Size = new System.Drawing.Size(57, 20);
            this.textBox17.TabIndex = 15;
            this.textBox17.Text = "-2.5";
            // 
            // textBox16
            // 
            this.textBox16.Enabled = false;
            this.textBox16.Location = new System.Drawing.Point(337, 91);
            this.textBox16.Name = "textBox16";
            this.textBox16.Size = new System.Drawing.Size(57, 20);
            this.textBox16.TabIndex = 14;
            this.textBox16.Text = "70";
            // 
            // label98
            // 
            this.label98.AutoSize = true;
            this.label98.Location = new System.Drawing.Point(99, 240);
            this.label98.Name = "label98";
            this.label98.Size = new System.Drawing.Size(19, 13);
            this.label98.TabIndex = 13;
            this.label98.Text = "c2";
            // 
            // label97
            // 
            this.label97.AutoSize = true;
            this.label97.Location = new System.Drawing.Point(100, 207);
            this.label97.Name = "label97";
            this.label97.Size = new System.Drawing.Size(19, 13);
            this.label97.TabIndex = 12;
            this.label97.Text = "c1";
            // 
            // label96
            // 
            this.label96.AutoSize = true;
            this.label96.Location = new System.Drawing.Point(101, 173);
            this.label96.Name = "label96";
            this.label96.Size = new System.Drawing.Size(19, 13);
            this.label96.TabIndex = 11;
            this.label96.Text = "k1";
            // 
            // textBox15
            // 
            this.textBox15.Location = new System.Drawing.Point(32, 238);
            this.textBox15.Name = "textBox15";
            this.textBox15.Size = new System.Drawing.Size(50, 20);
            this.textBox15.TabIndex = 10;
            this.textBox15.Text = "5";
            // 
            // textBox14
            // 
            this.textBox14.Location = new System.Drawing.Point(33, 205);
            this.textBox14.Name = "textBox14";
            this.textBox14.Size = new System.Drawing.Size(50, 20);
            this.textBox14.TabIndex = 9;
            this.textBox14.Text = "-0.5";
            // 
            // textBox13
            // 
            this.textBox13.Location = new System.Drawing.Point(32, 173);
            this.textBox13.Name = "textBox13";
            this.textBox13.Size = new System.Drawing.Size(50, 20);
            this.textBox13.TabIndex = 8;
            this.textBox13.Text = "0.0001";
            // 
            // label95
            // 
            this.label95.AutoSize = true;
            this.label95.Location = new System.Drawing.Point(98, 121);
            this.label95.Name = "label95";
            this.label95.Size = new System.Drawing.Size(19, 13);
            this.label95.TabIndex = 7;
            this.label95.Text = "b1";
            // 
            // label94
            // 
            this.label94.AutoSize = true;
            this.label94.Location = new System.Drawing.Point(97, 94);
            this.label94.Name = "label94";
            this.label94.Size = new System.Drawing.Size(20, 13);
            this.label94.TabIndex = 6;
            this.label94.Text = "P1";
            // 
            // textBox12
            // 
            this.textBox12.Location = new System.Drawing.Point(34, 120);
            this.textBox12.Name = "textBox12";
            this.textBox12.Size = new System.Drawing.Size(50, 20);
            this.textBox12.TabIndex = 5;
            this.textBox12.Text = "2";
            // 
            // textBox11
            // 
            this.textBox11.Location = new System.Drawing.Point(34, 91);
            this.textBox11.Name = "textBox11";
            this.textBox11.Size = new System.Drawing.Size(51, 20);
            this.textBox11.TabIndex = 4;
            this.textBox11.Text = "0.000053";
            // 
            // checkBox6
            // 
            this.checkBox6.AutoSize = true;
            this.checkBox6.Enabled = false;
            this.checkBox6.Location = new System.Drawing.Point(336, 61);
            this.checkBox6.Name = "checkBox6";
            this.checkBox6.Size = new System.Drawing.Size(127, 17);
            this.checkBox6.TabIndex = 3;
            this.checkBox6.Text = "Chemical Weathering";
            this.checkBox6.UseVisualStyleBackColor = true;
            // 
            // checkBox5
            // 
            this.checkBox5.AutoSize = true;
            this.checkBox5.Location = new System.Drawing.Point(35, 150);
            this.checkBox5.Name = "checkBox5";
            this.checkBox5.Size = new System.Drawing.Size(123, 17);
            this.checkBox5.TabIndex = 2;
            this.checkBox5.Text = "Physical Weathering";
            this.checkBox5.UseVisualStyleBackColor = true;
            // 
            // checkBox4
            // 
            this.checkBox4.AutoSize = true;
            this.checkBox4.Location = new System.Drawing.Point(33, 63);
            this.checkBox4.Name = "checkBox4";
            this.checkBox4.Size = new System.Drawing.Size(108, 17);
            this.checkBox4.TabIndex = 1;
            this.checkBox4.Text = "Bedrock lowering";
            this.checkBox4.UseVisualStyleBackColor = true;
            // 
            // soildevbox
            // 
            this.soildevbox.AutoSize = true;
            this.soildevbox.Location = new System.Drawing.Point(32, 16);
            this.soildevbox.Name = "soildevbox";
            this.soildevbox.Size = new System.Drawing.Size(147, 17);
            this.soildevbox.TabIndex = 0;
            this.soildevbox.Text = "Soil Development Model?";
            this.soildevbox.UseVisualStyleBackColor = true;
            // 
            // Panel1
            // 
            this.Panel1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.Panel1.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.Panel1.Controls.Add(this.tempdata2);
            this.Panel1.Controls.Add(this.tempdata1);
            this.Panel1.Controls.Add(this.graphicToGoogleEarthButton);
            this.Panel1.Controls.Add(this.button4);
            this.Panel1.Controls.Add(this.button3);
            this.Panel1.Controls.Add(this.recirculatebox);
            this.Panel1.Controls.Add(this.flowonlybox);
            this.Panel1.Location = new System.Drawing.Point(7, 476);
            this.Panel1.Name = "Panel1";
            this.Panel1.Size = new System.Drawing.Size(822, 49);
            this.Panel1.TabIndex = 145;
            this.Panel1.Visible = false;
            // 
            // tempdata2
            // 
            this.tempdata2.Location = new System.Drawing.Point(548, 7);
            this.tempdata2.Name = "tempdata2";
            this.tempdata2.Size = new System.Drawing.Size(39, 20);
            this.tempdata2.TabIndex = 145;
            this.tempdata2.Text = "358";
            // 
            // tempdata1
            // 
            this.tempdata1.Location = new System.Drawing.Point(503, 7);
            this.tempdata1.Name = "tempdata1";
            this.tempdata1.Size = new System.Drawing.Size(39, 20);
            this.tempdata1.TabIndex = 98;
            this.tempdata1.Text = "358";
            // 
            // graphicToGoogleEarthButton
            // 
            this.graphicToGoogleEarthButton.Location = new System.Drawing.Point(693, 8);
            this.graphicToGoogleEarthButton.Name = "graphicToGoogleEarthButton";
            this.graphicToGoogleEarthButton.Size = new System.Drawing.Size(119, 28);
            this.graphicToGoogleEarthButton.TabIndex = 150;
            this.graphicToGoogleEarthButton.Text = "graphic to google earth";
            this.graphicToGoogleEarthButton.UseVisualStyleBackColor = true;
            this.graphicToGoogleEarthButton.Click += new System.EventHandler(this.graphicToGoogleEarthButton_Click);
            // 
            // button4
            // 
            this.button4.Location = new System.Drawing.Point(8, 4);
            this.button4.Name = "button4";
            this.button4.Size = new System.Drawing.Size(112, 28);
            this.button4.TabIndex = 144;
            this.button4.Text = "update graphics";
            this.button4.Click += new System.EventHandler(this.button4_Click_1);
            // 
            // button3
            // 
            this.button3.Font = new System.Drawing.Font("Monotype Corsiva", 8.25F, System.Drawing.FontStyle.Italic, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.button3.Location = new System.Drawing.Point(604, 5);
            this.button3.Name = "button3";
            this.button3.Size = new System.Drawing.Size(82, 29);
            this.button3.TabIndex = 99;
            this.button3.Text = "Grass now!";
            this.button3.Click += new System.EventHandler(this.button3_Click);
            // 
            // recirculatebox
            // 
            this.recirculatebox.Location = new System.Drawing.Point(221, 6);
            this.recirculatebox.Name = "recirculatebox";
            this.recirculatebox.Size = new System.Drawing.Size(128, 24);
            this.recirculatebox.TabIndex = 89;
            this.recirculatebox.Text = "recirculate sediment";
            // 
            // flowonlybox
            // 
            this.flowonlybox.Location = new System.Drawing.Point(370, 5);
            this.flowonlybox.Name = "flowonlybox";
            this.flowonlybox.Size = new System.Drawing.Size(104, 24);
            this.flowonlybox.TabIndex = 97;
            this.flowonlybox.Text = "flow only?";
            // 
            // statusBar1
            // 
            this.statusBar1.Location = new System.Drawing.Point(0, 561);
            this.statusBar1.Name = "statusBar1";
            this.statusBar1.Panels.AddRange(new System.Windows.Forms.StatusBarPanel[] {
            this.InfoStatusPanel,
            this.IterationStatusPanel,
            this.TimeStatusPanel,
            this.QwStatusPanel,
            this.QsStatusPanel,
            this.tempStatusPanel});
            this.statusBar1.ShowPanels = true;
            this.statusBar1.Size = new System.Drawing.Size(1149, 22);
            this.statusBar1.SizingGrip = false;
            this.statusBar1.TabIndex = 144;
            this.statusBar1.Text = "statusBar1";
            // 
            // InfoStatusPanel
            // 
            this.InfoStatusPanel.Name = "InfoStatusPanel";
            this.InfoStatusPanel.Text = "info";
            this.InfoStatusPanel.Width = 200;
            // 
            // IterationStatusPanel
            // 
            this.IterationStatusPanel.Name = "IterationStatusPanel";
            this.IterationStatusPanel.Text = "iterations";
            this.IterationStatusPanel.Width = 120;
            // 
            // TimeStatusPanel
            // 
            this.TimeStatusPanel.Name = "TimeStatusPanel";
            this.TimeStatusPanel.Text = "time";
            this.TimeStatusPanel.Width = 120;
            // 
            // QwStatusPanel
            // 
            this.QwStatusPanel.Name = "QwStatusPanel";
            this.QwStatusPanel.Text = "Qw";
            this.QwStatusPanel.Width = 120;
            // 
            // QsStatusPanel
            // 
            this.QsStatusPanel.Name = "QsStatusPanel";
            this.QsStatusPanel.Text = "Qs";
            this.QsStatusPanel.Width = 120;
            // 
            // tempStatusPanel
            // 
            this.tempStatusPanel.Name = "tempStatusPanel";
            this.tempStatusPanel.Text = "tempdata";
            // 
            // start_button
            // 
            this.start_button.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.start_button.Enabled = false;
            this.start_button.Location = new System.Drawing.Point(114, 514);
            this.start_button.Name = "start_button";
            this.start_button.Size = new System.Drawing.Size(88, 25);
            this.start_button.TabIndex = 146;
            this.start_button.Text = "Start!";
            this.start_button.Click += new System.EventHandler(this.main_loop);
            // 
            // button1
            // 
            this.button1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.button1.Enabled = false;
            this.button1.Location = new System.Drawing.Point(207, 514);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(100, 25);
            this.button1.TabIndex = 147;
            this.button1.Text = "Quit and save";
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // checkBox2
            // 
            checkBox2.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            checkBox2.AutoSize = true;
            checkBox2.Location = new System.Drawing.Point(407, 543);
            checkBox2.Name = "checkBox2";
            checkBox2.Size = new System.Drawing.Size(108, 17);
            checkBox2.TabIndex = 151;
            checkBox2.Text = "point info window";
            checkBox2.UseVisualStyleBackColor = true;
            checkBox2.CheckedChanged += new System.EventHandler(this.checkBox2_CheckedChanged);
            // 
            // checkBox1
            // 
            this.checkBox1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.checkBox1.AutoSize = true;
            this.checkBox1.Checked = true;
            this.checkBox1.CheckState = System.Windows.Forms.CheckState.Checked;
            this.checkBox1.Location = new System.Drawing.Point(313, 522);
            this.checkBox1.Name = "checkBox1";
            this.checkBox1.Size = new System.Drawing.Size(77, 17);
            this.checkBox1.TabIndex = 151;
            this.checkBox1.Text = "view tabs?";
            this.checkBox1.UseVisualStyleBackColor = true;
            this.checkBox1.CheckedChanged += new System.EventHandler(this.checkBox1_CheckedChanged_1);
            // 
            // trackBar1
            // 
            this.trackBar1.AutoSize = false;
            this.trackBar1.Location = new System.Drawing.Point(216, 9);
            this.trackBar1.Name = "trackBar1";
            this.trackBar1.Size = new System.Drawing.Size(104, 20);
            this.trackBar1.TabIndex = 149;
            this.trackBar1.Scroll += new System.EventHandler(this.trackBar1_Scroll);
            // 
            // label61
            // 
            this.label61.AutoSize = true;
            this.label61.Location = new System.Drawing.Point(6, 14);
            this.label61.Name = "label61";
            this.label61.Size = new System.Drawing.Size(44, 13);
            this.label61.TabIndex = 150;
            this.label61.Text = "Graphic";
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.label62);
            this.groupBox2.Controls.Add(this.comboBox1);
            this.groupBox2.Controls.Add(this.trackBar1);
            this.groupBox2.Controls.Add(this.label61);
            this.groupBox2.Location = new System.Drawing.Point(12, -3);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(334, 35);
            this.groupBox2.TabIndex = 151;
            this.groupBox2.TabStop = false;
            this.groupBox2.Visible = false;
            // 
            // label62
            // 
            this.label62.AutoSize = true;
            this.label62.Location = new System.Drawing.Point(170, 14);
            this.label62.Name = "label62";
            this.label62.Size = new System.Drawing.Size(46, 13);
            this.label62.TabIndex = 152;
            this.label62.Text = "Contrast";
            // 
            // comboBox1
            // 
            this.comboBox1.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBox1.FormattingEnabled = true;
            this.comboBox1.Location = new System.Drawing.Point(54, 10);
            this.comboBox1.Name = "comboBox1";
            this.comboBox1.Size = new System.Drawing.Size(104, 21);
            this.comboBox1.TabIndex = 151;
            this.comboBox1.SelectedValueChanged += new System.EventHandler(this.comboBox1_SelectedValueChanged);
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.label63);
            this.groupBox3.Controls.Add(this.trackBar2);
            this.groupBox3.Location = new System.Drawing.Point(346, -3);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(156, 35);
            this.groupBox3.TabIndex = 152;
            this.groupBox3.TabStop = false;
            this.groupBox3.Visible = false;
            // 
            // label63
            // 
            this.label63.AutoSize = true;
            this.label63.Location = new System.Drawing.Point(6, 13);
            this.label63.Name = "label63";
            this.label63.Size = new System.Drawing.Size(34, 13);
            this.label63.TabIndex = 154;
            this.label63.Text = "Zoom";
            // 
            // trackBar2
            // 
            this.trackBar2.AutoSize = false;
            this.trackBar2.Location = new System.Drawing.Point(42, 9);
            this.trackBar2.Minimum = 1;
            this.trackBar2.Name = "trackBar2";
            this.trackBar2.Size = new System.Drawing.Size(104, 20);
            this.trackBar2.TabIndex = 153;
            this.trackBar2.Value = 5;
            this.trackBar2.Scroll += new System.EventHandler(this.trackBar2_Scroll);
            // 
            // backgroundWorker1
            // 
            this.backgroundWorker1.DoWork += new System.ComponentModel.DoWorkEventHandler(this.backgroundWorker1_DoWork);
            // 
            // zoomPanImageBox1
            // 
            this.zoomPanImageBox1.Image = null;
            this.zoomPanImageBox1.Location = new System.Drawing.Point(7, 37);
            this.zoomPanImageBox1.Name = "zoomPanImageBox1";
            this.zoomPanImageBox1.Size = new System.Drawing.Size(824, 316);
            this.zoomPanImageBox1.TabIndex = 148;
            this.zoomPanImageBox1.Visible = false;
            this.zoomPanImageBox1.Load += new System.EventHandler(this.zoomPanImageBox1_Load);
            // 
            // Form1
            // 
            this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
            this.AutoScroll = true;
            this.ClientSize = new System.Drawing.Size(1149, 583);
            this.Controls.Add(checkBox2);
            this.Controls.Add(this.checkBox1);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.tabControl1);
            this.Controls.Add(this.start_button);
            this.Controls.Add(this.button2);
            this.Controls.Add(this.statusBar1);
            this.Controls.Add(this.Panel1);
            this.Controls.Add(this.groupBox3);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.zoomPanImageBox1);
            this.Menu = this.mainMenu1;
            this.Name = "Form1";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "CAESAR Lisflood 1.9j";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.Resize += new System.EventHandler(this.Form1_Resize);
            this.tabControl1.ResumeLayout(false);
            this.FilesTab.ResumeLayout(false);
            this.FilesTab.PerformLayout();
            this.groupBox6.ResumeLayout(false);
            this.groupBox6.PerformLayout();
            this.groupBox4.ResumeLayout(false);
            this.groupBox4.PerformLayout();
            this.NumericalTab.ResumeLayout(false);
            this.NumericalTab.PerformLayout();
            this.GrainTab.ResumeLayout(false);
            this.GrainTab.PerformLayout();
            this.DescriptionTab.ResumeLayout(false);
            this.DescriptionTab.PerformLayout();
            this.GridTab.ResumeLayout(false);
            this.GridTab.PerformLayout();
            this.HydrologyTab.ResumeLayout(false);
            this.groupBox7.ResumeLayout(false);
            this.groupBox7.PerformLayout();
            this.groupBox5.ResumeLayout(false);
            this.groupBox5.PerformLayout();
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.tabPage2.ResumeLayout(false);
            this.tabPage2.PerformLayout();
            this.groupBox8.ResumeLayout(false);
            this.groupBox8.PerformLayout();
            this.tabPage4.ResumeLayout(false);
            this.tabPage4.PerformLayout();
            this.tabPage5.ResumeLayout(false);
            this.tabPage5.PerformLayout();
            this.tabPage1.ResumeLayout(false);
            this.tabPage1.PerformLayout();
            this.tabPage3.ResumeLayout(false);
            this.tabPage3.PerformLayout();
            this.Panel1.ResumeLayout(false);
            this.Panel1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.InfoStatusPanel)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.IterationStatusPanel)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.TimeStatusPanel)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.QwStatusPanel)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.QsStatusPanel)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.tempStatusPanel)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBar1)).EndInit();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.trackBar2)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

		}
		#endregion

		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]
		static void Main() 
		{
			Application.Run(new Form1());
		}

		private void start_button_Click(object sender, System.EventArgs e)
		{
			xtextbox.Visible=false;
			ytextbox.Visible=false;
			button1.Visible=false;
			button2.Visible=false;
			label1.Visible=false;
			label2.Visible=false;
			
		}

		private void clearforms()
		{
			tabControl1.Visible = false;
            
		}

		private void main_loop(object sender, System.EventArgs e)
		{

            
			/********* locals ***********/
			double tempflow=baseflow;
			double ince=cycle+60;
			
            gameClock = new System.Windows.Forms.Timer();
            
//			clearforms();                  // MJ 14/01/05   this is done when load button is clicked
			start_button.Enabled = false;  // MJ 17/01/05
			button1.Enabled = true;        // MJ 17/01/05 
            comboBox1.Items.Add("water depth");
            popComboBox1();

			time_1=1;

			calc_J(1.0);

			save_time=cycle;
			creep_time=cycle;
            creep_time2 = cycle;
            soil_erosion_time = cycle;
            soil_development_time = cycle;
			time_1=cycle;

			

			mygraphics = this.CreateGraphics();

			
            //slide_5();
            get_area();

            get_catchment_input_points();

			this.InfoStatusPanel.Text="Starting main loop";        // MJ 14/01/05
			//JMW 

			this.InfoStatusPanel.Text="Running";        // MJ 14/01/05
            
            //Googel Earth Animation Directory
            if (googleAnimationCheckbox.Checked)
            {
                if(Directory.Exists("animation"))Directory.Delete("animation",true);
                Directory.CreateDirectory("animation");
            }
			
            time_factor = 1;

			// Here we go into the main loop,
			// I copied this bit of code to get
			// the event handler to work - and it seems OK so why change it.
			// so the main loop is really in erodedepo()

            gameClock.Interval = 100;
            gameClock.Tick += new EventHandler(update_screen_outputs);
            gameClock.Start();

            backgroundWorker1.RunWorkerAsync();
            
		}

        private void erodedepo() // erodedepo is the main loop from which all the main CL functions are called.
		{
            var watch = System.Diagnostics.Stopwatch.StartNew();

			int y;
            do
            {
                
                // set of lines commented out to instigate uplift at set times (if needed)
                // line to do uplift at set time....
                //if (cycle > (1440 * 18250) && elev[1, 1] < 100)
                //{
                //    for (int x = 1; x <= 300; x++)
                //    {
                //        for (y = 1; y <= ymax; y++)
                //        {
                //            elev[x, y] += 10;//Convert.ToInt32(textBox6.Text);
                //        }
                //    }
                //    slide_5();
                //    slide_5();
                //    elev[1, 1] = 100;
                //}

                // Gez code: set previous cycle = to cycle
                previous = cycle;
                old_cycle = cycle % output_file_save_interval;
                // end gez code

                //
                // section below workign out time step.
                //
                double input_output_difference = Math.Abs(waterinput - waterOut);
                //this.tempdata1.Text = Convert.ToString(waterinput-waterOut);
             
                // all dealing with calculating model time step
                if (maxdepth <= water_depth_erosion_threshold) maxdepth = water_depth_erosion_threshold;
                //if (time_factor < min_time_step) time_factor = min_time_step;
                if (time_factor < (courant_number * (DX / Math.Sqrt(gravity * (maxdepth))))) time_factor = (courant_number * (DX / Math.Sqrt(gravity * (maxdepth))));
                if (input_output_difference > in_out_difference && time_factor > (courant_number * (DX / Math.Sqrt(gravity * (maxdepth)))))
                {
                    time_factor = courant_number * (DX / Math.Sqrt(gravity * (maxdepth)));
                }

                double local_time_factor = time_factor;
                if (local_time_factor > (courant_number * (DX / Math.Sqrt(gravity * (maxdepth))))) local_time_factor = courant_number * (DX / Math.Sqrt(gravity * (maxdepth)));

                

                // code to incrememtn counters. Counter is model iterations, cycle is the actual (modelled reality) time
                counter++;
                cycle += time_factor / 60;
                // cycle in min
                // time_factor in secs.

                // Gez code
                new_cycle = cycle % output_file_save_interval;
                // end gez code


                //
                // adding in sand movement gradually
                if (DuneBox.Checked == true && cycle >= duneupdatetime)
                {
                    double factor = (max_time_step/60) / double.Parse(dune_time_box.Text);
                    //factor = 1;
                    for (int x = 1; x <= xmax; x++)
                    {
                        for (y = 1; y <= ymax; y++)
                        {
                            if (elev_diff[x, y] != 0)
                            {
                                if (elev[x, y] < init_elevs[x, y] && elev_diff[x, y] > 0) elev_diff[x, y] = 0;
                                elev[x, y] -= elev_diff[x, y] * factor;
                                //elev_diff[x, y] = 0;
                                if (index[x, y] != -9999 && elev_diff[x, y] < 0 && water_depth[x, y] >= water_depth_erosion_threshold)
                                {
                                    grain[index[x, y], 1] -= elev_diff[x, y] * factor;
                                    sand[x, y] += elev_diff[x, y] * factor;
                                    sort_active(x, y);
                                }
                            }
                        }
                    }
                    duneupdatetime = cycle + (max_time_step/60);
                }


                ////			This whole section below deals with the water inputs. A separate part for point inputs and 
                ////		    a different bit for gradual inputs from the whole of the catchment. (reach or catchment)
                // first zero counter to tally up water inputs
                waterinput = 0;

                // reach mode water inputs
                if (reach_mode_box.Checked == true) reach_water_and_sediment_input(local_time_factor);

                // catchment mode water inputs
                if (catchment_mode_box.Checked == true) catchment_water_input_and_hydrology(local_time_factor);

                // tidal/stage mode water inputs
                if (checkBox3.Checked == true) stage_tidal_input(local_time_factor);

                // route water and update flow depths

                qroute();
                depth_update();

                // check scan area every 5 iters.. maybe re-visit for reach mode if it causes too much backing up of sed. see code commented below nex if..
                if (Math.IEEERemainder(counter, 5) == 0)
                {
                    scan_area();
                }

                // carry out erosion

                if (!flowonlybox.Checked)
                {
                    if (counter >= erode_call)
                    {
                        erode_mult = (int)(ERODEFACTOR / erode(erode_mult));
                        if (erode_mult < 1) erode_mult = 1;
                        if (erode_mult > 10) erode_mult = 10;

                        erode_call = counter + erode_mult;
                        //this.tempdata1.Text = Convert.ToString(erode_mult);
                    }
                    //
                    // carry out lateral erosion
                    //
                    if (newlateral.Checked && counter >= lateralcounter)
                    {
                        lateral3();
                        lateralcounter = counter + (50 * erode_mult);
                    }
                }

                //
                // work out water coming out..
                // and zero water depths in edges..
                //
                // this doesnt actually zero water - sets it to the min water depth
                // If this is not done, then zero'd water depth prevents material being moved to the edge cells and thus eroded fromthe catchment
                // leading to buildups of sediment at the edge.

                double temptot = 0;
                for (y = 1; y <= ymax; y++)
                {
                    if (water_depth[xmax, y] > water_depth_erosion_threshold)
                    {
                        temptot += (water_depth[xmax, y]-water_depth_erosion_threshold) * DX * DX / local_time_factor;
                        // and zero water depths at RH end
                        water_depth[xmax, y] = water_depth_erosion_threshold;
                    }
                    if (water_depth[1, y] > water_depth_erosion_threshold)
                    {
                        temptot += (water_depth[1, y]-water_depth_erosion_threshold) * DX * DX / local_time_factor;
                        // and zero water depths at RH end
                        water_depth[1, y] = water_depth_erosion_threshold;
                    }
                }
                for (int x = 1; x <= xmax; x++)
                {
                    if (water_depth[x, 1] > water_depth_erosion_threshold)
                    {
                        temptot += (water_depth[x, 1]-water_depth_erosion_threshold) * DX * DX / local_time_factor;
                        // and zero water depths at RH end
                        water_depth[x, 1] = water_depth_erosion_threshold;
                    }
                    if (water_depth[x, ymax] > water_depth_erosion_threshold)
                    {
                        temptot += (water_depth[x, ymax]-water_depth_erosion_threshold) * DX * DX / local_time_factor;
                        // and zero water depths at RH end
                        water_depth[x, ymax] = water_depth_erosion_threshold;
                    }
                }
                waterOut = temptot;

                // then carry out soil creep. 
                //- also growing grass... 
                // and dunes...
                // slide_3 is looking at landslides only in the 'scanned' area
                // slide_4 is to do with dunes
                // slide_5 is landslides everywhere..

                if (!flowonlybox.Checked)
                {
                    // carry out local landslides every X iterations...
                    if (Math.IEEERemainder(counter, 10) == 0)
                    {
                        slide_3();
                    }
                    // soil creep - here every 10 days
                    if (cycle > creep_time)
                    {
                        // updating counter creep_time
                        creep_time += 14400;
                        // now calling soil creep function
                        creep(0.028);//(0.019);//(0.083);
                    }
                    //// doing sand dunes part

                    if (DuneBox.Checked == true && cycle > dune_time)
                    {
                        dune1(0);
                        dune_time += double.Parse(dune_time_box.Text); ;
                    }

                    //
                    // now calling soil erosion function  - also checks if soil erosion rate is > 0 
                    // to prevent it calling it if not required.
                    // then soil erosion time
                    //

                    if (SOIL_RATE > 0 && cycle > soil_erosion_time)
                    {
                        get_area(); // gets drainage area before doing soil erosion - as used in the calcs. therefore good to have accurate/fresh drianage area
                        soil_erosion_time += 1440;// do soil erosion daily
                        //if (soilerosionBox.Checked == true) SOIL_RATE = ((0.768383841078 * j_mean + 0.000001979564));
                        soilerosion(0.0028);
                    }

                    if (cycle > soil_development_time)
                    {
                        soil_development_time += 1440 * 365 / 12;
                        if (soildevbox.Checked) soil_development();
                    }

                }


                
                // other slope things.. done every day.
                if (cycle > creep_time2)
                {
                    evaporate(1440);
                    // updating counter creep_time2
                    creep_time2 += 1440;// daily

                    if (!flowonlybox.Checked)
                    {

                        slide_5();

                        // calling siberia model (if wanted)
                        if (SiberiaBox.Checked == true)
                        {
                            get_area();
                            siberia(0.002739); // the value passed is the time step (in years)
                        }
                        // next line does grass growing, must change it if changes from monthly update
                        if (grow_grass_time > 0) grow_grass(1 / (grow_grass_time * 365));

                    }
                }

                // Gez
                temptotal = temptot;

                // call output routine
                // only if iteration by iteration output is required.
                if (checkBoxGenerateTimeSeries.Checked)  // MJ 20/01/05
                {
                    output_data();
                }
                // end gez code

                //Google Earth Animation outputs data to file.
                if (cycle >= save_time2 && googleAnimationCheckbox.Checked)
                {
                    googleoutputflag = true;
                    save_time2 += save_interval2;
                    imageCount2 = imageCount2 + 1;
                }

                // save data & draw graphics every specified interval. Passes the cycle whuch becomes the file extension
                if (cycle >= save_time && uniquefilecheck.Checked == true)
                {
                    save_data_and_draw_graphics();
                    save_time += saveinterval;
                }
                // if its at the end of the run kill the program
            } while (cycle / 60 < maxcycle);

            watch.Stop();
            MessageBox.Show($"finished. Runtime: {watch.Elapsed} ");
            // end of main loop!!
		}
    

        void catchment_water_input_and_hydrology(double local_time_factor)
        {
            // revised way of determining water input 21/8/18
            double tempwaterinput = 0;

            for (int z = 1; z <= totalinputpoints; z++)
            {
                int x = catchment_input_x_coord[z];
                int y = catchment_input_y_coord[z];
                double water_add_amt = (j_mean[rfarea[x, y]] * nActualGridCells[rfarea[x, y]]) / (catchment_input_counter[rfarea[x, y]]) * local_time_factor;
                // revised way of getting water input 21/8/18
                tempwaterinput += water_add_amt * DX * DX / local_time_factor;
                if (water_add_amt > ERODEFACTOR) water_add_amt = ERODEFACTOR;
                
                water_depth[x, y] += water_add_amt;
                //if ((j_mean * nActualGridCells) / (catchment_input_counter) * local_time_factor > ERODEFACTOR) this.tempStatusPanel.Text = "C add= " + Convert.ToString((j_mean * nActualGridCells) / (catchment_input_counter) * local_time_factor);
            }

            waterinput = tempwaterinput;

            // if the input type flag is 1 then the discharge is input from the hydrograph...
            if (cycle >= time_1)
            {
                do
                {
                    time_1++;
                    calc_J(time_1);

                    if (time_factor > max_time_step)
                    {

                        double j_mean_max_temp = 0;
                        for (int n = 1; n <= rfnum; n++) if (new_j_mean[n] > j_mean_max_temp) j_mean_max_temp = new_j_mean[n];
                        if (j_mean_max_temp > (0.2 / (xmax * ymax * DX * DX)))
                        // check after variable rainfall area has been added...
                        //stops code going too fast when there is actual flow in the channels greater than 0.2cu.
                        {
                            cycle = time_1 + (max_time_step / 60);
                            time_factor = max_time_step;
                        }
                    }
                } while (time_1 < cycle);
            }

            calchydrograph(time_1 - cycle);

            double jmeanmax = 0;
            for (int n = 1; n <= rfnum; n++) if (j_mean[n] > jmeanmax) jmeanmax = j_mean[n];

            if (jmeaninputfilebox.Checked == true) j_mean[1] = ((hourly_rain_data[(int)(cycle / rain_data_time_step), 1]) / Math.Pow(DX, 2)) / nActualGridCells[1];

            if (jmeanmax >= baseflow)
            {
                baseflow = baseflow * 3;
                //Console.Write("going up.. \n");
                get_area();
                get_catchment_input_points();
            }


            if (baseflow > (jmeanmax * 3) && baseflow > 0.000001)
            {
                baseflow = jmeanmax * 1.25;
                //Console.Write("going down.. \n");
                get_area();
                get_catchment_input_points();
            }

            
        }

        void reach_water_and_sediment_input(double local_time_factor)
        {
            double[] remove_from_temp_grain;

            remove_from_temp_grain = new Double[G_MAX + 2];

            if (reach_mode_box.Checked == true)
            {
                for (int n = 0; n <= number_of_points - 1; n++)
                {
                    int tempn;

                    int x = inpoints[n, 0];
                    int y = inpoints[n, 1];



                    double adding_factor = 1;
                    // tot up total to be added - and if its greater than number of cells and erode_limit
                    // then reduce what can be added via a factor. 
                    double added_tot = 0;

                    if (water_depth[x, y] > water_depth_erosion_threshold)
                    {
                        for (tempn = 5; tempn <= G_MAX+3; tempn++)
                        {
                            added_tot += (Math.Abs(inputfile[n, (int)(cycle / input_time_step), tempn])) / div_inputs / (DX * DX) / (input_time_step * 60) * time_factor;
                        }

                        // then multiply by the recirculation factor..
                        if (added_tot / number_of_points > ERODEFACTOR * 0.75) adding_factor = (ERODEFACTOR * 0.75) / (added_tot / number_of_points);

                        if (index[x, y] == -9999) addGS(x, y);
                        for (tempn = 5; tempn <= G_MAX+3; tempn++)
                        {
                            double amount_to_add = adding_factor * (Math.Abs(inputfile[n, (int)(cycle / input_time_step), tempn])) / div_inputs / (DX * DX) / (input_time_step * 60) * time_factor;
                            if (isSuspended[tempn - 4])
                            {
                                Vsusptot[x, y] += amount_to_add;
                            }
                            else
                            {
                                grain[index[x, y], tempn - 4] += amount_to_add;
                                elev[x, y] += amount_to_add;
                            }

                        }
                    }
                }
            }

            // add in recirculating sediment.
            if (recirculatebox.Checked == true && reach_mode_box.Checked == true)
            {
                int tempn;
                double adding_factor = 1;
                // tot up total to be added - and if its greater than number of cells and erode_limit
                // then reduce what can be added via a factor. 
                double added_tot = 0;

                for (tempn = 5; tempn <= G_MAX+3 ; tempn++)
                {
                    added_tot += temp_grain[tempn - 4];
                    remove_from_temp_grain[tempn - 4] = 0; // quick way of emptying this variable
                }

                // then multiply by the recirculation factor..
                if (added_tot / number_of_points > ERODEFACTOR * 0.75) adding_factor = (ERODEFACTOR * 0.75) / (added_tot / number_of_points);


                    for (int n = 0; n <= number_of_points - 1; n++)
                    {
                        int x = inpoints[n, 0];
                        int y = inpoints[n, 1];

                        if (water_depth[x, y] > water_depth_erosion_threshold)
                        {

                        for (tempn = 5; tempn <= G_MAX+3 ; tempn++)
                        {
                            if (index[x, y] == -9999) addGS(x, y);
                            if (isSuspended[tempn - 4])
                            {
                                Vsusptot[x, y] += ((temp_grain[tempn - 4] * adding_factor) / number_of_points);
                                remove_from_temp_grain[tempn - 4] += ((temp_grain[tempn - 4] * adding_factor) / number_of_points);
                            }
                            else
                            {
                                grain[index[x, y], tempn - 4] += ((temp_grain[tempn - 4] * adding_factor) / number_of_points);
                                elev[x, y] += ((temp_grain[tempn - 4] * adding_factor) / number_of_points);
                                remove_from_temp_grain[tempn - 4] += ((temp_grain[tempn - 4] * adding_factor) / number_of_points);
                            }

                        }
                    }
                }

            }

            for (int n = 1; n <= G_MAX; n++)
            {
                temp_grain[n] -= remove_from_temp_grain[n];
            }
            


            for (int n = 0; n <= number_of_points - 1; n++)
            {
                int x = inpoints[n, 0];
                int y = inpoints[n, 1];
                double interpolated_input1 = inputfile[n, (int)(cycle / input_time_step), 1];
                double interpolated_input2 = inputfile[n, (int)(cycle / input_time_step) + 1, 1];
                double proportion_between_time1and2 = ((((int)(cycle / input_time_step)+ 1 ) * input_time_step) - cycle)
                    / input_time_step;


                double input = interpolated_input1 + ((interpolated_input2 - interpolated_input1) * (1-proportion_between_time1and2));

                //j_mean = old_j_mean + (((new_j_mean - old_j_mean) / 2) * (2 - time));
                
                waterinput += (input / div_inputs);

                // trial adding SS line
                // if(counter<500)Vsusptot[x+5, y] = 0.1;
                water_depth[x, y] += (input / div_inputs) / (DX * DX) * local_time_factor;
                // also have to add suspended sediment here..
                // from file

            }



        }

        void stage_tidal_input(double local_time_factor)
        {

            for (int x = Math.Min(fromx, tox); x <= Math.Max(fromx, tox); x++)
            {
                for (int y = Math.Min(fromy, toy); y <= Math.Max(fromy, toy); y++)
                {
                    double interpolated_input1 = stage_inputfile[(int)(cycle / stage_input_time_step)];
                    double interpolated_input2 = stage_inputfile[(int)(cycle / stage_input_time_step) + 1];
                    double proportion_between_time1and2 = ((((int)(cycle / stage_input_time_step) + 1) * stage_input_time_step) - cycle)
                        / stage_input_time_step;


                    double input = interpolated_input1 + ((interpolated_input2 - interpolated_input1) * (1 - proportion_between_time1and2));

                    if (elev[x, y] > -9999 && input > elev[x, y])
                    {
                        water_depth[x, y] = input - elev[x, y];
                        if (water_depth[x, y] > 0) Vsusptot[x, y] = water_depth[x, y] * 0.001;//0.0005 is 500mg l.. approx.
                    }
                }

            }

        }

        void soil_development() // all based on Vanwalleghem et al., 2013 (JoGR:ES)
        {
            for (int x = 1; x <= xmax; x++)
            {
                for (int y = 1; y <= ymax; y++)
                {
                    if (elev[x, y] > -9999) // ensure it is not a no-data point
                    {
                        if (index[x, y] == -9999) addGS(x, y); // first ensure that there is grainsize defined for that cell

                        if (checkBox4.Checked) // Bedrock lowering
                        {
                            if (bedrock[x, y] > -9999 && elev[x,y]>=bedrock[x,y])
                            {
                                double h = elev[x, y] - bedrock[x, y];
                                if (h == 0) h = 0.001;
                                bedrock[x,y] += - P1 * Math.Exp(-b1 * (h)) / 12; //  / 12 to make it months
                            }
                        }

                        if (checkBox5.Checked) // Physical Weathering
                        {
                            //amt = k1 * Math.Exp(-c1*depth_below_surface) * (c2/Math.Log10(particle_size) * time;
                            // start from top then work down - means no need to hold values in temp arrays...
                            double Di = 0;
                            int xyindex = index[x, y];


                            for (int n = 2; n <= (G_MAX - 1); n++)
                            {
                                if ((grain[xyindex, n] > 0.0))
                                {
                                    switch (n)
                                    {
                                        case 1: Di = d1; break;
                                        case 2: Di = d2; break;
                                        case 3: Di = d3; break;
                                        case 4: Di = d4; break;
                                        case 5: Di = d5; break;
                                        case 6: Di = d6; break;
                                        case 7: Di = d7; break;
                                        case 8: Di = d8; break;
                                        case 9: Di = d9; break;
                                    }

                                    double amount = grain[xyindex, n] * ((-(k1 * Math.Exp(-c1 * active * 0.5) * (c2 / Math.Log(Di)) * 1)) / 12); //  / 12 to make it months
                                    grain[xyindex, n] -= amount;
                                    if (n == 2)
                                    {
                                        grain[xyindex, n - 1] += amount;
                                    }
                                    else
                                    {
                                        grain[xyindex, n - 1] += amount * 0.05;
                                        grain[xyindex, n - 2] += amount * 0.95;
                                    }

                                    for (int z = 1; z <= 9; z++)
                                    {
                                        double amount2 = strata[xyindex, z - 1, n] * ((-(k1 * Math.Exp(-c1 * active * z) * (c2 / Math.Log(Di)) * 1)) / 12); //  / 12 to make it months

                                        strata[xyindex, z-1, n] -= amount;
                                        if (n == 2)
                                        {
                                            strata[xyindex, z-1, n - 1] += amount;
                                        }
                                        else
                                        {
                                            strata[xyindex, z-1, n - 1] += amount * 0.05;
                                            strata[xyindex, z-1, n - 2] += amount * 0.95;
                                        }

           
                                    }
                                }
                            }

                            //for (int z = 9; z >= 1; z--)
                            //{
                            //    for (int n = 0; n <= G_MAX - 2; n++)
                            //    {

                            //        strata[xyindex, z, n] = strata[xyindex, z - 1, n];

                            //    }
                            //}


                        }

                        if (checkBox6.Checked) // Chemical Weathering
                        {
                            //amt = k2 * Math.Exp(-c3 * depth_below_surface) * c4 * Specific_surface_area * time;
                        }
                    }
                }
            }
        }

        void get_catchment_input_points()
        {
            totalinputpoints = 0;
            for (int n = 1; n <= rfnum; n++) catchment_input_counter[n] = 0;
            for (int x = 1; x <= xmax; x++)
            {
                for (int y = 1; y <= ymax; y++)
                {
                    if ((area[x, y] * baseflow * DX * DX) > MIN_Q && (area[x, y] * baseflow * DX * DX) < Convert.ToDouble(MinQmaxvalue.Text))
                    {
                        totalinputpoints++;
                        catchment_input_x_coord[totalinputpoints] = x;
                        catchment_input_y_coord[totalinputpoints] = y;
                        catchment_input_counter[rfarea[x, y]]++;
                    }
                }
            }
            // could be the below line causing problems..?
            //if (totalinputpoints == 0) totalinputpoints = 1;
        }

        void evaporate(double time)
        {
            double evap_amount = k_evap * (time / 1440);
            // now reduce if greater than erodedepo - to prevent instability
            if (evap_amount > ERODEFACTOR) evap_amount = ERODEFACTOR;

            var options = new ParallelOptions { MaxDegreeOfParallelism = Environment.ProcessorCount *  4 };
            Parallel.For(1, ymax, options, delegate(int y)
            {
                int inc = 1;
                while (down_scan[y, inc] > 0)
                {
                    int x = down_scan[y, inc];
                    inc++;
                    // removes water in rate of mm per day..
                    if (water_depth[x, y] > 0)
                    {
                        water_depth[x, y] -= evap_amount;
                        if (water_depth[x, y] < 0) water_depth[x, y] = 0;
                    }

                }

            });
        }

        void save_data_every1000iters()
        {
            if (menuItem25.Checked == true) save_data(1, 0); // save waterdepths
            if (menuItem13.Checked == true) save_data(2, 0); // save elevdiff
            if (menuItem12.Checked == true) save_data(3, 0); // save elevations
            if (menuItem14.Checked == true) save_data(4, 0); // save grainsize
            if (menuItem29.Checked == true) save_data(15, 0); // save d50 top layer
            if (menuItem33.Checked == true) save_data(16, 0); // save velocity	<JOE 20050605>
            if (menuItem34.Checked == true) save_data(17, 0); // save soil_saturation	<JOE 20050605>
            slide_5();

        }

        void save_data_and_draw_graphics()
        {
            if (menuItem25.Checked == true) save_data(1, Math.Abs(cycle)); // save waterdepths
            if (menuItem13.Checked == true) save_data(2, Math.Abs(cycle)); // save elevdiff
            if (menuItem12.Checked == true) save_data(3, Math.Abs(cycle)); // save elevations
            if (menuItem14.Checked == true) save_data(4, Math.Abs(cycle)); // save grainsize
            if (menuItem29.Checked == true) save_data(15, Math.Abs(cycle)); // save d50 top layer
            if (menuItem33.Checked == true) save_data(16, Math.Abs(cycle)); // save velocity			<JOE 20050605>
            if (menuItem34.Checked == true) save_data(17, Math.Abs(cycle)); // save soil saturation	<JOE 20050605

        }

        void update_screen_outputs(object sender, System.EventArgs e)
        {
            this.IterationStatusPanel.Text = "it = " + counter.ToString();  // MJ 14/01/05
            if (cycle <= 1440)
            {
                this.TimeStatusPanel.Text = string.Format("t = {0:F6} min", cycle);
            }
            else
            {
                this.TimeStatusPanel.Text = string.Format("t = {0:F6} day", cycle / 1440);
            }

         
            // now display sedi out bottom..
            if (menuItem4.Checked == false) this.QsStatusPanel.Text = string.Format("Qs = {0:F8}", sediQ); // MJ 14/01/05
            // updates the text boxes with the time and iterationbs elapsed
            this.QwStatusPanel.Text = string.Format("Qw = {0:G}", waterOut); // MJ 14/01/05

            if(googleoutputflag)
            {
                googleoutputflag = false;
                updateClick = 1;
                this.Refresh();
                drawwater(mygraphics);
                if (coordinateDone == 0)
                {
                    //transfrom coordinates
                    point testPoint = new point(xll, yll);
                    if (UTMgridcheckbox.Checked)
                    {
                        testPoint.UTMzone = System.Convert.ToInt32(UTMzonebox.Text);
                        testPoint.south = System.Convert.ToBoolean(UTMsouthcheck.Checked);
                        testPoint.transformUTMPoint();
                    }
                    else
                    {
                        testPoint.transformPoint();
                    }
                    yurcorner = yll + (System.Convert.ToDouble(ymax) * System.Convert.ToDouble(DX));
                    xurcorner = xll + (System.Convert.ToDouble(xmax) * System.Convert.ToDouble(DX));
                    point testPoint2 = new point(xurcorner, yurcorner);
                    if (UTMgridcheckbox.Checked)
                    {
                        testPoint2.UTMzone = System.Convert.ToInt32(UTMzonebox.Text);
                        testPoint2.south = System.Convert.ToBoolean(UTMsouthcheck.Checked);
                        testPoint2.transformUTMPoint();
                    }
                    else
                    {
                        testPoint2.transformPoint();
                    }
                    urfinalLati = testPoint2.ycoord;
                    urfinalLongi = testPoint2.xcoord;
                    llfinalLati = testPoint.ycoord;
                    llfinalLongi = testPoint.xcoord;
                    coordinateDone = 1;
                }
                //Save image
                m_objDrawingSurface.MakeTransparent();
                m_objDrawingSurface.Save("animation\\mysavedimage" + imageCount2 + ".png", System.Drawing.Imaging.ImageFormat.Png);
                //update time
                googleTime = googleTime.AddMinutes(save_interval2);
                kmlTime = googleTime.ToString();
                DateArray = kmlTime.Split(new char[] { ' ' });
                DateArray2 = DateArray[0].Split(new char[] { '/' });
                kmlTime = DateArray2[2] + "-" + DateArray2[1] + "-" + DateArray2[0] + "T" + DateArray[1] + "Z";

                //create kml file for image
                StreamWriter kmlsr = File.AppendText(KML_FILE_NAME);
                if (imageCount2 == 1)
                {
                    kml = @"<?xml version=""1.0"" encoding=""UTF-8""?>
                         <kml xmlns=""http://earth.google.com/kml/2.1"">";
                    kml = kml + "\n<Folder>"
                        + "\n<name>Animation</name>";
                    kmlsr.WriteLine(kml);
                    kml = "";
                }
                kml = kml + "\n<GroundOverlay>"
                    + "\n<name>Untitled Image Overlay</name>";
                kml = kml + "\n<TimeSpan>"
                       + "\n<begin>" + kmlTime + "</begin>"
                       + "\n<end>" + kmlTime + "</end>"
                       + "\n</TimeSpan>"
                       + "\n<Icon>"
                       + "\n<href>mySavedImage" + imageCount2 + ".png</href>"
                       + "\n</Icon>"
                       + "\n<LatLonBox>";
                kml = kml + "\n<north>" + urfinalLati + "</north>"
                      + "\n<south>" + llfinalLati + "</south>"
                      + "\n<east>" + urfinalLongi + "</east>"
                      + "\n<west>" + llfinalLongi + "</west>\n";
                kml = kml + @"</LatLonBox>
                           </GroundOverlay>";
                kmlsr.WriteLine(kml);
                kml = "";
                kmlsr.Close();
                //imageCount2 = imageCount2 + 1;
                //save_time2 += save_interval2;
            }
        }

        void calchydrograph(double time)
        {
            for (int n = 1; n <= rfnum; n++)
            {
                j_mean[n] = old_j_mean[n] + (((new_j_mean[n] - old_j_mean[n]) / 2) * (2 - time));
            }
        }
		


		void output_data()
		{
			int n;

			// Qw (m3) value at timestep cycle (current
			// Qw_newvol+=temptotal*((cycle-previous)*output_file_save_interval); // replaced by line below MJ 25/01/05
			Qw_newvol+=temptotal*((cycle-previous)*60);  // 60 secs per min
			for(int nn=1;nn<=rfnum;nn++) Jw_newvol += (j_mean[nn]*DX*DX*nActualGridCells[nn]) * ((cycle-previous)*60);

			//Catch all time steps that pass one or more hour marks 
			if ((new_cycle<old_cycle)||(cycle-previous>=output_file_save_interval))
			{
				while ((tx>previous)&&(cycle>=tx))
				{
					hours++;
			
					// Step1: Calculate hourly total sediment Q (m3)
					Qs_step = globalsediq-old_sediq;
					Qs_over = Qs_step*((cycle-tx)/(cycle-tlastcalc));
					Qs_hour = Qs_step-Qs_over+Qs_last;

					// reset Qs_last and old_sediq for large time steps
					if (cycle>=tx+output_file_save_interval)
					{
						Qs_last = 0;
						old_sediq = globalsediq - Qs_over;
					}

					// reset Qs_last and old_sediq for small time steps
					if (cycle<tx+output_file_save_interval)
					{
						Qs_last = Qs_over;
						old_sediq = globalsediq;
					}

					// Step 2: Calculate grain size Qgs, also calculate contaminated amounts
					for (n=1;n<=G_MAX-1;n++)
					{
						// calculate timestep Qgs
						Qg_step[n] = sum_grain[n]-old_sum_grain[n];
                        Qg_step2[n] = sum_grain2[n] - old_sum_grain2[n];
						// Interpolate Qgs beyond time tx
						Qg_over[n] = Qg_step[n]*((cycle-tx)/(cycle-tlastcalc));
                        Qg_over2[n] = Qg_step2[n] * ((cycle - tx) / (cycle - tlastcalc));
						// and calculate hourly Qgs
						Qg_hour[n] = Qg_step[n]-Qg_over[n]+Qg_last[n];
                        Qg_hour2[n] = Qg_step2[n] - Qg_over2[n] + Qg_last2[n];
						// Reset Qg_last[n] and old_sum_grain[n]for large time steps
						if (cycle>=tx+output_file_save_interval)
						{
							Qg_last[n] = 0;
                            Qg_last2[n] = 0;
							old_sum_grain[n] = sum_grain[n] - Qg_over[n];
                            old_sum_grain2[n] = sum_grain2[n] - Qg_over2[n];
						}
						// Reset Qg_last[n] and old_sum_grain[n] for small time steps
						if (cycle<tx+output_file_save_interval)
						{
							Qg_last[n] = Qg_over[n];
                            Qg_last2[n] = Qg_over2[n];
							old_sum_grain[n] = sum_grain[n];
                            old_sum_grain2[n] = sum_grain2[n];
						}
					}
			
					// Step 3: Calculate hourly mean water discharge
					// Qw_overvol = temptotal*((cycle-tx)*output_file_save_interval); // replaced by line below MJ 25/01/05
					Qw_overvol = temptotal*((cycle-tx)*60);	// 60 secs per min
					Qw_stepvol = Qw_newvol-Qw_oldvol;
					Qw_hourvol = Qw_stepvol-Qw_overvol+Qw_lastvol;
					Qw_hour = Qw_hourvol/(60*output_file_save_interval); // convert hourly water volume to cumecs

					// same for Jw (j_mean contribution)  MJ 14/03/05
					for(int nn=1;nn<=rfnum;nn++) Jw_overvol = (j_mean[nn]*DX*DX*nActualGridCells[nn])*((cycle-tx)*60);  // fixed MJ 29/03/05
					Jw_stepvol = Jw_newvol-Jw_oldvol;
					Jw_hourvol = Jw_stepvol-Jw_overvol+Jw_lastvol;
					Jw_hour = Jw_hourvol/(60*output_file_save_interval);


					// reset Qw_lastvol and Qw_oldvol for large time steps
					if (cycle>=tx+output_file_save_interval)
					{
						Qw_lastvol = 0;
						Qw_oldvol = Qw_newvol-Qw_overvol;

						// same for Jw (j_mean contribution)  MJ 14/03/05
						Jw_lastvol = 0;
						Jw_oldvol = Jw_newvol-Jw_overvol;
					}

					// reset Qw_lastvol and Qw_oldvol for small time steps
					if (cycle<tx+output_file_save_interval)
					{
						Qw_lastvol = Qw_overvol;
						Qw_oldvol = Qw_newvol;

						// same for Jw (j_mean contribution)  MJ 14/03/05
						Jw_lastvol = Jw_overvol;
						Jw_oldvol = Jw_newvol;
					}

					Tx = tx;
					tx = Tx+output_file_save_interval;
                  
			
					// Step 4: Output hourly data to file (format for reach model input)
					// changed MJ 18/01/05
					string output = string.Format("{0}",hours);
					output = output	+ string.Format(" {0:F6}",Qw_hour);
					output = output	+ string.Format(" {0:F6}",Jw_hour);
                    if (SiberiaBox.Checked == true)
                    {
                        double tomsedi =0;
                        for (int x = 1; x <= xmax; x++)
                        {
                            for (int y = 1; y <= ymax; y++)
                            {
                                if (elev[x, y] > -9999)
                                {
                                    tomsedi += (init_elevs[x, y] - elev[x, y]) * DX * DX;
                                }
                            }
                        }
                        output = output + string.Format(" {0:F6}", tomsedi);
                    }
                    else
                    {
                        output = output + string.Format(" {0:F6}", sand_out);
                        sand_out = 0;
                    }
					output = output	+ string.Format(" {0:F10}",Qs_hour);
					for (n=1;n<=G_MAX-1;n++)
					{
						output = output + string.Format(" {0:F10}",Qg_hour[n]);
						//output = output+" "+Qg_hour[n];
					}

                    try
                    {
                        StreamWriter sw = File.AppendText(CATCH_FILE);
                        sw.WriteLine(output);
                        sw.Close();
                    }
                    catch (Exception)
                    {

                    }


				}
				tlastcalc = cycle;	
			}													   
		}


		int read_header()
		{
			string FILE_NAME;
			int z;
			string[] lineArray2;
			int sp;

			inputheader = new string[6];

			FILE_NAME = this.openfiletextbox.Text;
			if (!File.Exists(FILE_NAME)) 
			{
				MessageBox.Show("No such DEM data file..");
				return 0;
			}

			try
			{

				//read headers
				StreamReader sr = File.OpenText(FILE_NAME);
				for(z = 1; z <= 6; z++)
				{
					inputheader[z-1] = sr.ReadLine();
				}
				sr.Close();

				// get xmax, ymax and DX from input headers

				lineArray2 = inputheader[0].Split(new char[]{' '});
				sp = 1;
				while (lineArray2[sp] == "") sp++;
				xmax = int.Parse(lineArray2[sp]);

				lineArray2 = inputheader[1].Split(new char[]{' '});
				sp = 1;
				while (lineArray2[sp] == "") sp++;
				ymax = int.Parse(lineArray2[sp]);

                lineArray2 = inputheader[2].Split(new char[] { ' ' });
                sp = 1;
                while (lineArray2[sp] == "") sp++;
                xll = double.Parse(lineArray2[sp]);

                lineArray2 = inputheader[3].Split(new char[] { ' ' });
                sp = 1;
                while (lineArray2[sp] == "") sp++;
                yll = double.Parse(lineArray2[sp]);

				lineArray2 = inputheader[4].Split(new char[]{' '});
				sp = 1;
				while (lineArray2[sp] == "") sp++;
				DX = double.Parse(lineArray2[sp]);
                root = (Math.Sqrt(Math.Pow(DX, 2) + Math.Pow(DX, 2))); // imp added 11/07
               
			}			
			catch(Exception)
			{
				MessageBox.Show("Theres a problem with the header in the DEM file");
			}

			return 1;
		}


		void load_data()
		{
			int x,y=1,z,xcounter,x1=0,y1=0,n;			
			String input;
			double tttt=0.00;

            char[] delimiterChars = {' ',',','\t'};

			// load dem

			string FILE_NAME = this.openfiletextbox.Text;
			if (!File.Exists(FILE_NAME)) 
			{
				MessageBox.Show("No such DEM data file..");
				return;
			}

			StreamReader sr = File.OpenText(FILE_NAME);

			//read headers
			for(z=1;z<=6;z++)
			{
				input=sr.ReadLine();
			}
			y=1;

			while ((input=sr.ReadLine())!=null) 
			{
				string[] lineArray;
				lineArray = input.Split(delimiterChars);
				xcounter=1;
				for (x = 0; x<=(lineArray.Length-1); x++)
				{
					
					if(lineArray[x]!=""&&xcounter<=xmax)
					{
						tttt=double.Parse(lineArray[x]);
						elev[xcounter,y]=tttt;
						//if(xcounter==1)elev[xcounter,y]+=4;
						init_elevs[xcounter,y]=elev[xcounter,y];
						xcounter++;
					}
				}
				y++;

			}
			sr.Close();

            // load hydro coverage
            ///////////////////////////////////////

            try
            {
                FILE_NAME = this.hydroindexBox.Text;
                if (FILE_NAME != "null")
                {
                    sr = File.OpenText(FILE_NAME);

                    //read headers
                    for (z = 1; z <= 6; z++)
                    {
                        input = sr.ReadLine();
                    }
                    y = 1;

                    while ((input = sr.ReadLine()) != null)
                    {
                        string[] lineArray;
                        lineArray = input.Split(delimiterChars);
                        xcounter = 1;
                        for (x = 0; x <= (lineArray.Length - 1); x++)
                        {

                            if (lineArray[x] != "" && xcounter <= xmax)
                            {
                                int xxxx = int.Parse(lineArray[x]);
                                if (xxxx == -9999) xxxx = 1;
                                rfarea[xcounter, y] = xxxx;
                                xcounter++;
                            }
                        }
                        y++;

                    }
                    sr.Close();
                }
            }
            catch (Exception)
            {
 
            }

			if(this.graindataloadbox.Text!="null")
			{
				FILE_NAME = this.graindataloadbox.Text;
				if(FILE_NAME!="null")
				{
					StreamReader gr = File.OpenText(FILE_NAME);
					y=1;
					grain_array_tot=0;
					while ((input=gr.ReadLine())!=null) 
					{
						string[] lineArray;
						lineArray = input.Split(delimiterChars);
						xcounter=1;
						grain_array_tot++;
						for (x = 0; x<=(lineArray.Length-1); x++)
						{
							if(lineArray[x]!="")
							{
								if(xcounter==1)x1=int.Parse(lineArray[x]);
								if(xcounter==2)y1=int.Parse(lineArray[x]);
                                if (x1 > xmax) x1 = xmax; // lines to prevent adding grain if clipping the grid and excellss on the grain grid is left
                                if (y1 > ymax) y1 = ymax; //

                                if (xcounter == 3) index[x1, y1] = grain_array_tot;// int.Parse(lineArray[x]);

								for(n=0;n<=G_MAX;n++)
								{
									if(xcounter==4+n)
									{
										grain[grain_array_tot,n]=double.Parse(lineArray[x]);
									}
								}

								for(z=0;z<=9;z++)
								{
									for(n=0;n<=(G_MAX-2);n++)
									{
										if(xcounter==(4+G_MAX+n+1)+((z)*9))
										{
											strata[grain_array_tot,z,n]=double.Parse(lineArray[x]);
										}
									}
								}

								xcounter++;
							}
						}
					}
					gr.Close();
				}
			}
			

			FILE_NAME = this.bedrockbox.Text;
			if(FILE_NAME!="null")
			{

				StreamReader gr = File.OpenText(FILE_NAME);
				//read headers
				for(z=1;z<=6;z++)
				{
					input=gr.ReadLine();
				}
				y=1;

				while ((input=gr.ReadLine())!=null) 
				{
                    string[] lineArray;
                    lineArray = input.Split(delimiterChars);
                    xcounter = 1;
                    for (x = 0; x <= (lineArray.Length - 1); x++)
                    {

                        if (lineArray[x] != "" && xcounter <= xmax)
                        {
                            tttt = double.Parse(lineArray[x]);
                            bedrock[xcounter, y] = tttt;
                            xcounter++;
                        }
                    }
                    y++;

				}
				gr.Close();
			}

            int inc1 = 1;

            try
            {


                FILE_NAME = this.raindataloadbox.Text;
                if (FILE_NAME != "null")
                {


                    //int inc = 1;
                    StreamReader gr = File.OpenText(FILE_NAME);
                    while ((input = gr.ReadLine()) != null)
                    {
                        string[] lineArray;
                        lineArray = input.Split(delimiterChars);
                        xcounter = 1;
                        //MessageBox.Show(Convert.ToString(inc));
                        for (x = 0; x <= (lineArray.Length - 1); x++)
                        {
                            if (lineArray[x] != "" )
                            {
                                tttt = double.Parse(lineArray[x]);
                                hourly_rain_data[inc1, xcounter] = tttt;

                                xcounter++;
                            }
                        }
                        inc1++;
                    }

                    gr.Close();
                 }
            }

            catch (Exception)
            {
                MessageBox.Show("There was some type of error loading the input data from the rain data file at line " + Convert.ToString(inc1) + ", CAESAR will continue to function but may not be correct");
                //MessageBox.Show(" There was some type of error loading the input data from the rain data file CAESAR may continue to function but may not be correct");
            }

            try
            {
                //FILE_NAME = this.mvalueloadbox.Text;
                //if (FILE_NAME != "null")
                //{
                //    variable_m_value_flag = 1; // sets flag for variable M value to 1 (true)
                //    int inc = 1;
                //    StreamReader gr = File.OpenText(FILE_NAME);
                //    while ((input = gr.ReadLine()) != null)
                //    {
                //        hourly_m_value[inc] = double.Parse(input);
                //        inc++;
                //    }
                //    gr.Close();
                //}
                FILE_NAME = this.mvalueloadbox.Text;
                if (FILE_NAME != "null")
                {
                    variable_m_value_flag = 1; // sets flag for variable M value to 1 (true)
                    int inc = 1;
                    StreamReader gr = File.OpenText(FILE_NAME);
                    while ((input = gr.ReadLine()) != null)
                    {
                        string[] lineArray;
                        lineArray = input.Split(delimiterChars);
                        xcounter = 1;
                        //MessageBox.Show(Convert.ToString(inc));
                        for (x = 0; x <= (lineArray.Length - 1); x++)
                        {
                            if (lineArray[x] != "")
                            {
                                tttt = double.Parse(lineArray[x]);
                                hourly_m_value[inc, xcounter] = tttt;
                                xcounter++;
                            }
                        }
                        inc++;
                    }

                    gr.Close();
                }
            }
            catch (Exception)
            {
                MessageBox.Show("There was some type of error loading the m value data from the m value data file CAESAR may continue to function but may not be correct");
            }

            if (checkBox3.Checked == true)
            {
                try
                {
                    FILE_NAME = this.TidalFileName.Text;
                    if (FILE_NAME != "null")
                    {
                        int inc = 1;
                        StreamReader gr = File.OpenText(FILE_NAME);
                        while ((input = gr.ReadLine()) != null)
                        {
                            stage_inputfile[inc] = double.Parse(input);
                            inc++;
                        }
                        gr.Close();
                    }
                }
                catch (Exception)
                {
                    MessageBox.Show("There was some type of error loading the stage/tidal data file CAESAR may continue to function but may not be correct");
                }
            }

            // load spatially variable mannings
            ///////////////////////////////////////

            if (SpatVarManningsCheckbox.Checked == true)
            {
                try
                {
                    FILE_NAME = this.textBox19.Text;
                    if (FILE_NAME != "null")
                    {
                        sr = File.OpenText(FILE_NAME);

                        //read headers
                        for (z = 1; z <= 6; z++)
                        {
                            input = sr.ReadLine();
                        }
                        y = 1;

                        while ((input = sr.ReadLine()) != null)
                        {
                            string[] lineArray;
                            lineArray = input.Split(delimiterChars);
                            xcounter = 1;
                            for (x = 0; x <= (lineArray.Length - 1); x++)
                            {

                                if (lineArray[x] != "" && xcounter <= xmax)
                                {
                                    double xxxx = double.Parse(lineArray[x]);
                                    if (xxxx == -9999) xxxx = 0.04; //failsafe to stop nodatas giving strange results.
                                    spat_var_mannings [xcounter, y] = xxxx;
                                    xcounter++;
                                }
                            }
                            y++;

                        }
                        sr.Close();
                    }
                }
                catch (Exception)
                {

                }
            }

            //try
            //{
            //    //single mine input "supermine", up to nine grainsizes
            //    if (mine_checkBox.Checked)
            //    {
            //        //mine_inpoints[0, 0] = Convert.ToInt32(mineX_textBox.Text); //input x coordrinate 
            //        //mine_inpoints[0, 1] = Convert.ToInt32(mineY_textBox.Text); //input y coordinate
            //        FILE_NAME = this.mine_input_textBox.Text;
            //        if (FILE_NAME != "null")
            //        {
            //            int lineinc = 1;//number of hours of contamination
            //            StreamReader gr = File.OpenText(FILE_NAME);
            //            while ((input = gr.ReadLine()) != null)
            //            {
            //                int inc = 1;//grainsizes 1-9, only two necessary now

            //                string[] lineArray;
            //                lineArray = input.Split(new char[] { ' ' });
            //                for (x = 1; x < (lineArray.Length); x++)
            //                {
            //                    if (lineArray[x] != "")
            //                    {
            //                        //if (inc == 1) mineinput[lineinc, inc] = double.Parse(lineArray[x]);//grainsize1
            //                        //if (inc == 2) mineinput[lineinc, inc] = double.Parse(lineArray[x]);//grainsize2
            //                        //if (inc == 3) mineinput[lineinc, inc] = double.Parse(lineArray[x]);//grainsize3
            //                        //if (inc == 4) mineinput[lineinc, inc] = double.Parse(lineArray[x]);//grainsize4
            //                        //if (inc == 5) mineinput[lineinc, inc] = double.Parse(lineArray[x]);//grainsize5
            //                        //if (inc == 6) mineinput[lineinc, inc] = double.Parse(lineArray[x]);//grainsize6
            //                        //if (inc == 7) mineinput[lineinc, inc] = double.Parse(lineArray[x]);//grainsize7
            //                        //if (inc == 8) mineinput[lineinc, inc] = double.Parse(lineArray[x]);//grainsize8
            //                        //if (inc == 9) mineinput[lineinc, inc] = double.Parse(lineArray[x]);//grainsize9
            //                        inc++;
            //                    }
            //                }
            //                lineinc++;
            //            }
            //            gr.Close();
            //        }
            //    }
            //}
            //catch (Exception ex)
            //{
            //    MessageBox.Show("There was some type of error loading the contaminant input CAESAR may continue to function but may not be correct");
            //}
            int tempx=0;

			try
			{
				if(inbox1.Checked)
				{
					FILE_NAME = this.infile1.Text;
					if(FILE_NAME!="null")
					{
						input_type_flag=0;
						StreamReader gr = File.OpenText(FILE_NAME);
						while ((input=gr.ReadLine())!=null) 
						{
							string[] lineArray;
							lineArray = input.Split(delimiterChars);
							for (x = 0; x<=(lineArray.Length-1); x++)
							{
								if(lineArray[x]!="")
								{
									tempx=int.Parse(lineArray[0]);
									inputfile[0,tempx,x]=double.Parse(lineArray[x]);
								}
							}
						}
						gr.Close();
					}

				}
            }catch(Exception)
			{
				MessageBox.Show("There was some type of error loading the input data from "+FILE_NAME+" at line "+Convert.ToString(tempx)+", CAESAR will continue to function but may not be correct");
			}

            try{
				if(inbox2.Checked)
				{
					FILE_NAME = this.infile2.Text;
					if(FILE_NAME!="null")
					{
						input_type_flag=0;
						StreamReader gr = File.OpenText(FILE_NAME);
						while ((input=gr.ReadLine())!=null) 
						{
							string[] lineArray;
							lineArray = input.Split(delimiterChars);
							for (x = 0; x<=(lineArray.Length-1); x++)
							{
								if(lineArray[x]!="")
								{
									tempx=int.Parse(lineArray[0]);
									inputfile[1,tempx,x]=double.Parse(lineArray[x]);
								}
							}
						}
						gr.Close();
					}
				}
            }
            catch (Exception)
            {
                MessageBox.Show("There was some type of error loading the input data from " + FILE_NAME + " at line " + Convert.ToString(tempx) + ", CAESAR will continue to function but may not be correct");
            }

            try{
				if(inbox3.Checked)
				{
					FILE_NAME = this.infile3.Text;
					if(FILE_NAME!="null")
					{
						input_type_flag=0;
						StreamReader gr = File.OpenText(FILE_NAME);
						while ((input=gr.ReadLine())!=null) 
						{
							string[] lineArray;
							lineArray = input.Split(delimiterChars);
							for (x = 0; x<=(lineArray.Length-1); x++)
							{
								if(lineArray[x]!="")
								{
									tempx=int.Parse(lineArray[0]);
									inputfile[2,tempx,x]=double.Parse(lineArray[x]);
								}
							}
						}
						gr.Close();
					}
				}
            }
            catch (Exception)
            {
                MessageBox.Show("There was some type of error loading the input data from " + FILE_NAME + " at line " + Convert.ToString(tempx) + ", CAESAR will continue to function but may not be correct");
            }
            try{
				if(inbox4.Checked)
				{
					FILE_NAME = this.infile4.Text;
					if(FILE_NAME!="null")
					{
						input_type_flag=0;
						StreamReader gr = File.OpenText(FILE_NAME);
						while ((input=gr.ReadLine())!=null) 
						{
							string[] lineArray;
							lineArray = input.Split(delimiterChars);
							for (x = 0; x<=(lineArray.Length-1); x++)
							{
								if(lineArray[x]!="")
								{
									tempx=int.Parse(lineArray[0]);
									inputfile[3,tempx,x]=double.Parse(lineArray[x]);
								}
							}
						}
						gr.Close();
					}
				}
            }
            catch (Exception)
            {
                MessageBox.Show("There was some type of error loading the input data from " + FILE_NAME + " at line " + Convert.ToString(tempx) + ", CAESAR will continue to function but may not be correct");
            }

            try{
				if(inbox5.Checked)
				{
					FILE_NAME = this.infile5.Text;
					if(FILE_NAME!="null")
					{
						input_type_flag=0;
						StreamReader gr = File.OpenText(FILE_NAME);
						while ((input=gr.ReadLine())!=null) 
						{
							string[] lineArray;
							lineArray = input.Split(delimiterChars);
							for (x = 0; x<=(lineArray.Length-1); x++)
							{
								if(lineArray[x]!="")
								{
									tempx=int.Parse(lineArray[0]);
									inputfile[4,tempx,x]=double.Parse(lineArray[x]);
								}
							}
						}
						gr.Close();
					}
				}
            }
            catch (Exception)
            {
                MessageBox.Show("There was some type of error loading the input data from " + FILE_NAME + " at line " + Convert.ToString(tempx) + ", CAESAR will continue to function but may not be correct");
            }
            try{

				if(inbox6.Checked)
				{
					FILE_NAME = this.infile6.Text;
					if(FILE_NAME!="null")
					{
						input_type_flag=0;
						StreamReader gr = File.OpenText(FILE_NAME);
						while ((input=gr.ReadLine())!=null) 
						{
							string[] lineArray;
							lineArray = input.Split(delimiterChars);
							for (x = 0; x<=(lineArray.Length-1); x++)
							{
								if(lineArray[x]!="")
								{
									tempx=int.Parse(lineArray[0]);
									inputfile[5,tempx,x]=double.Parse(lineArray[x]);
								}
							}
						}
						gr.Close();
					}
				}
            }
            catch (Exception)
            {
                MessageBox.Show("There was some type of error loading the input data from " + FILE_NAME + " at line " + Convert.ToString(tempx) + ", CAESAR will continue to function but may not be correct");
            }
            try{
				if(inbox7.Checked)
				{
					FILE_NAME = this.infile7.Text;
					if(FILE_NAME!="null")
					{
						input_type_flag=0;
						StreamReader gr = File.OpenText(FILE_NAME);
						while ((input=gr.ReadLine())!=null) 
						{
							string[] lineArray;
							lineArray = input.Split(delimiterChars);
							for (x = 0; x<=(lineArray.Length-1); x++)
							{
								if(lineArray[x]!="")
								{
									tempx=int.Parse(lineArray[0]);
									inputfile[6,tempx,x]=double.Parse(lineArray[x]);
								}
							}
						}
						gr.Close();
					}
				}
            }
            catch (Exception)
            {
                MessageBox.Show("There was some type of error loading the input data from " + FILE_NAME + " at line " + Convert.ToString(tempx) + ", CAESAR will continue to function but may not be correct");
            }

            try{
				if(inbox8.Checked)
				{
					FILE_NAME = this.infile8.Text;
					if(FILE_NAME!="null")
					{
						input_type_flag=0;
						StreamReader gr = File.OpenText(FILE_NAME);
						while ((input=gr.ReadLine())!=null) 
						{
							string[] lineArray;
							lineArray = input.Split(delimiterChars);
							for (x = 0; x<=(lineArray.Length-1); x++)
							{
								if(lineArray[x]!="")
								{
									tempx=int.Parse(lineArray[0]);
									inputfile[7,tempx,x]=double.Parse(lineArray[x]);
								}
							}
						}
						gr.Close();
					}
				}
			}
            catch(Exception)
			{
				MessageBox.Show("There was some type of error loading the input data from "+FILE_NAME+" at line "+Convert.ToString(tempx)+", CAESAR will continue to function but may not be correct");
			}


            // code to try and draw in graphics from google maps etc...
            //string latlng = "39.24382,9.143372";
            //string path = "http://maps.googleapis.com/maps/api/staticmap?center=" + latlng +
            //   "&zoom=10&size=400x400&maptype=satellite&markers=color:blue%7Clabel:S%7C" +
            //   latlng + "&sensor=false";

            //try
            //{
            //    // bing maps instructions here: http://msdn.microsoft.com/en-us/library/ff701724.aspx
            //    // example line: http://dev.virtualearth.net/REST/v1/Imagery/Map/imagerySet?mapArea=mapArea&mapSize=mapSize&pushpin=pushpin&mapLayer=mapLayer&format=format&mapMetadata=mapMetadata&key=BingMapsKey

            //    point testPoint = new point(xll, yll);
            //    if (UTMgridcheckbox.Checked)
            //    {
            //        testPoint.UTMzone = System.Convert.ToInt32(UTMzonebox.Text);
            //        testPoint.south = System.Convert.ToBoolean(UTMsouthcheck.Checked);
            //        testPoint.transformUTMPoint();
            //    }
            //    else
            //    {
            //        testPoint.transformPoint();
            //    }
            //    yurcorner = yll + (System.Convert.ToDouble(ymax) * System.Convert.ToDouble(DX));
            //    xurcorner = xll + (System.Convert.ToDouble(xmax) * System.Convert.ToDouble(DX));
            //    point testPoint2 = new point(xurcorner, yurcorner);
            //    if (UTMgridcheckbox.Checked)
            //    {
            //        testPoint2.UTMzone = System.Convert.ToInt32(UTMzonebox.Text);
            //        testPoint2.south = System.Convert.ToBoolean(UTMsouthcheck.Checked);
            //        testPoint2.transformUTMPoint();
            //    }
            //    else
            //    {
            //        testPoint2.transformPoint();
            //    }

            //    string box = testPoint.ycoord + "," + testPoint.xcoord + "," + testPoint2.ycoord + "," + testPoint2.xcoord;
            //    //string box = "54.3397646119184,-2.30842643099455,54.4619251486287,-1.95376438512343";
            //    string path = "http://dev.virtualearth.net/REST/v1/Imagery/Map/Aerial?mapArea=" + box + "&mapSize=" + xmax + "," + ymax + "&key=AvaXZOU-oYf4A4vjryoLQEv06rQy9V3BBJkb7Bi4v8HaXoUTdDZevDbqtFEe9mi_";

            //    using (WebClient wc = new WebClient())
            //    {
            //        wc.DownloadFile(path, @"img.png");
            //    }
            //}
            //catch 
            //{
            //    MessageBox.Show("Time out trying to download background image: Check web connection");
            //}


			          			// MessageBox.Show(Convert.ToString(input_type_flag));
			this.InfoStatusPanel.Text="Loaded: type = " + input_type_flag.ToString();        // MJ 02/02/05			

		}


		void save_data(int typeflag, double tempcycle)
		{
			int x,y,z,inc,  nn;
			
			string FILENAME = "waterdepth.dat";

			if(uniquefilecheck.Checked==false)tempcycle=0;

			// turns file name into days from mins.
			


			if(typeflag==1&&tempcycle==0) FILENAME = "waterdepth.txt";
			if(typeflag==2&&tempcycle==0) FILENAME = "elevdiff.txt";
			if(typeflag==3&&tempcycle==0) FILENAME = "elev.txt";
            if(typeflag==4&&tempcycle==0) FILENAME = "grain.txt";
            if (typeflag == 15 && tempcycle == 0) FILENAME = "d50top.txt";
            if (typeflag == 16 && tempcycle == 0) FILENAME = "velocity.txt";			// <JOE 20050605>
            if (typeflag == 17 && tempcycle == 0) FILENAME = "velocity_vectors.txt";	// <JOE 20050605>

			if(typeflag==1&&tempcycle>0) FILENAME = "waterdepth"+Convert.ToString(Convert.ToInt64(tempcycle))+".txt";
            if (typeflag == 2 && tempcycle > 0) FILENAME = "elevdiff" + Convert.ToString(Convert.ToInt64(tempcycle)) + ".txt";
            if (typeflag == 3 && tempcycle > 0) FILENAME = "elev.dat" + Convert.ToString(Convert.ToInt64(tempcycle)) + ".txt";
            if (typeflag == 4 && tempcycle > 0) FILENAME = "grain.dat" + Convert.ToString(Convert.ToInt64(tempcycle)) + ".txt";

            if (typeflag == 15 && tempcycle > 0) FILENAME = "d50top" + Convert.ToString(Convert.ToInt64(tempcycle)) + ".txt";
            if (typeflag == 16 && tempcycle > 0) FILENAME = "velocity" + Convert.ToString(Convert.ToInt64(tempcycle)) + ".txt";
            if (typeflag == 17 && tempcycle > 0) FILENAME = "velocity_vectors" + Convert.ToString(Convert.ToInt64(tempcycle)) + ".txt";

			if(typeflag>=1&&typeflag<4)
			{
				
				using (StreamWriter sw = new StreamWriter(FILENAME)) 
				{
					sw.Write("ncols         "+xmax);
					sw.Write("\n");
					sw.Write("nrows         "+ymax);
					sw.Write("\n");

					for (nn=2; nn<=5; nn++)
					{
						sw.Write(inputheader[nn]);sw.Write("\n");
					}

					for(y=1;y<=ymax;y++)
					{
						for(x=1;x<=xmax;x++)
						{
							if(typeflag==1)
							{
                                //sw.Write(area[x, y]); sw.Write(" ");
                                if (water_depth[x, y] > 0)
                                {
                                    //sw.Write(vel_dir[x,y,0]);
                                    sw.Write(water_depth[x,y]);

                                    sw.Write(" ");
                                }
                                else
                                {
                                    sw.Write("-9999.0");
                                    sw.Write(" ");
                                }
                            }

							if(typeflag==2)
							{
								sw.Write(init_elevs[x,y]-elev[x,y]);
								sw.Write(" ");
							}
							if(typeflag==3)
							{
								sw.Write(elev[x,y]);
								sw.Write(" ");
							}
						}
						sw.Write("\n");
					}
					

				}
				
			}

			if(typeflag==4)
			{
				
				using (StreamWriter sw = new StreamWriter(FILENAME)) 
				{
					for (y=1;y<=ymax;y++)
					{
						for(x=1;x<=xmax;x++)
						{


							if(index[x,y]!=-9999)
							{
								sw.Write(x);sw.Write(" ");sw.Write(y);sw.Write(" ");sw.Write(index[x,y]);sw.Write(" ");
								for(inc=0;inc<=G_MAX;inc++)
								{
									sw.Write(grain[index[x,y],inc]);sw.Write(" ");
								}						
														
								for(z=0;z<=9;z++)
								{
									for(inc=0;inc<=(G_MAX-2);inc++)
									{
										sw.Write(strata[index[x,y],z,inc]);sw.Write(" ");
									}
								}
								sw.Write("\n");
							}
						}
					}
				}
			}


			if(typeflag==15)
			{
				
				using (StreamWriter sw = new StreamWriter(FILENAME)) 
				{
					for (nn=0; nn<=5; nn++)
					{
						sw.Write(inputheader[nn]);sw.Write("\n");
					}

					for(y=1;y<=ymax;y++)
					{
						for(x=1;x<=xmax;x++)
						{
							if(index[x,y] == -9999)
							{
								sw.Write("-9999.0");
								sw.Write(" ");
							}
							else
							{
								sw.Write(d50(index[x,y]));
								sw.Write(" ");
							}
						}
						sw.Write("\n");
					}

				}
			}

			if(typeflag==16)   // <JOE 20050605> Write velocity to a file
			{
				
				using (StreamWriter sw = new StreamWriter(FILENAME)) 
				{
					for (nn=0; nn<=5; nn++)
					{
						sw.Write(inputheader[nn]);sw.Write("\n");
					}

					for(y=1;y<=ymax;y++)
					{
						for(x=1;x<=xmax;x++)
						{
							if(water_depth[x,y]<water_depth_erosion_threshold)
							{
								sw.Write("-9999.0");
								sw.Write(" ");
							}
							else
							{
								sw.Write(Vel[x,y]);
								sw.Write(" ");
							}
						}
						sw.Write("\n");
					}

				}
			}

            if (typeflag == 17)   // <JOE 20050605> Write soil_saturation to a file
            {

                using (StreamWriter sw = new StreamWriter(FILENAME))
                {
                    //for (nn=0; nn<=5; nn++)
                    //{
                    //    sw.Write(inputheader[nn]);sw.Write("\n");
                    //}

                    sw.Write("x,y,rot,vel");
                    sw.Write("\n");

                    for (y = 1; y <= ymax; y++)
                    {
                        for (x = 1; x <= xmax; x++)
                        {
                            if (water_depth[x, y] > water_depth_erosion_threshold)
                            {
                                sw.Write((xll + (x * DX)) + (DX / 2)); sw.Write(","); sw.Write(((yll + ((ymax - y) * DX)))); sw.Write(","); sw.Write(calc_max_flow_direction_degrees(x, y)); sw.Write(","); sw.Write(Vel[x, y]);
                                sw.Write("\n");
                            }

                        }

                    }

                }
            }


		}


		private void initialise()
		{

			this.InfoStatusPanel.Text="Initialising..";			// MJ 14/01/05

			if (overrideheaderBox.Checked == true)
			{
				// input xmax, ymax and DX from text boxes  
			xmax=int.Parse(xtextbox.Text);
			ymax=int.Parse(ytextbox.Text);
				DX=double.Parse(dxbox.Text);             // now in read_header()  MJ 24/05/05
			}


            save_interval2 = int.Parse(googAnimationSaveInterval.Text);
            save_time2 = save_interval2;
			
			in_out_difference=double.Parse(initscansbox.Text);
			ERODEFACTOR=double.Parse(erodefactorbox.Text);
			root=(Math.Sqrt(Math.Pow(DX,2)+Math.Pow(DX,2)));
			LIMIT=int.Parse(limitbox.Text);
			MIN_Q=double.Parse(minqbox.Text);
			CREEP_RATE=double.Parse(creepratebox.Text);
            SOIL_RATE = double.Parse(soil_ratebox.Text);
			bed_proportion=double.Parse(lateralratebox.Text);

			cycle=double.Parse(textBox1.Text)*60;
			
			maxcycle=int.Parse(cyclemaxbox.Text);
			failureangle=double.Parse(slopebox.Text);
			
			saveinterval=double.Parse(saveintervalbox.Text);
			//M=double.Parse(mvaluebox.Text);
			grow_grass_time=double.Parse(grasstextbox.Text);
			output_file_save_interval=int.Parse(outputfilesaveintervalbox.Text);
			tx=output_file_save_interval;
			min_time_step=double.Parse(mintimestepbox.Text);
			active=double.Parse(activebox.Text);
			k_evap=double.Parse(k_evapBox.Text);  // added MJ 15/03/05
			vegTauCrit=double.Parse(vegTauCritBox.Text);  // added MJ 10/05/05
            max_time_step = int.Parse(max_time_step_Box.Text);	
            water_depth_erosion_threshold = double.Parse(Q2box.Text);
            max_vel = double.Parse(max_vel_box.Text);
            courant_number = double.Parse(courantbox.Text);
            hflow_threshold = double.Parse(textBox4.Text);
            lateral_cross_channel_smoothing = double.Parse(textBox7.Text);
            froude_limit = double.Parse(textBox8.Text);
            mannings = double.Parse(textBox9.Text);

            rfnum = int.Parse(rfnumBox.Text);

            if (googleAnimationCheckbox.Checked)
            {
                startDate = googleBeginDate.Text;
                googleTime = System.DateTime.Parse(googleBeginDate.Text);
            }
			d1=double.Parse(g1box.Text);
			d2=double.Parse(g2box.Text);
			d3=double.Parse(g3box.Text);
			d4=double.Parse(g4box.Text);
			d5=double.Parse(g5box.Text);
			d6=double.Parse(g6box.Text);
			d7=double.Parse(g7box.Text);
			d8=double.Parse(g8box.Text);
			d9=double.Parse(g9box.Text);

            dprop = new double[11];

                dprop[1] = double.Parse(gp1box.Text);
                dprop[2] = double.Parse(gp2box.Text);
                dprop[3] = double.Parse(gp3box.Text);
                dprop[4] = double.Parse(gp4box.Text);
                dprop[5] = double.Parse(gp5box.Text);
                dprop[6] = double.Parse(gp6box.Text);
                dprop[7] = double.Parse(gp7box.Text);
                dprop[8] = double.Parse(gp8box.Text);
                dprop[9] = double.Parse(gp9box.Text);


            G_MAX=10;
            if (dprop[9] == 0) G_MAX = 9;
            if (dprop[8] == 0) G_MAX = 8;
            if (dprop[7] == 0) G_MAX = 7;
            if (dprop[6] == 0) G_MAX = 6;
            if (dprop[5] == 0) G_MAX = 5;
            if (dprop[4] == 0) G_MAX = 4;
            if (dprop[3] == 0) G_MAX = 3;
            if (dprop[2] == 0) G_MAX = 2;
            if (checkBox8.Checked == true) G_MAX = 10;


			// new array to deal with suspended sediment   MJ 09/05/05
			isSuspended = new bool[10+1];
			isSuspended[1] = suspGS1box.Checked;
			isSuspended[2] = false;
            isSuspended[3] = false;
            isSuspended[4] = false;
            isSuspended[5] = false;
            isSuspended[6] = false;
            isSuspended[7] = false;
            isSuspended[8] = false;
            isSuspended[9] = false;

			// new array to deal with suspended sediment   MJ 09/05/05
			fallVelocity = new double[10+1];
			fallVelocity[1] = double.Parse(fallGS1box.Text);
			fallVelocity[2] = double.Parse(fallGS2box.Text);
			fallVelocity[3] = double.Parse(fallGS3box.Text);
			fallVelocity[4] = double.Parse(fallGS4box.Text);
			fallVelocity[5] = double.Parse(fallGS5box.Text);
			fallVelocity[6] = double.Parse(fallGS6box.Text);
			fallVelocity[7] = double.Parse(fallGS7box.Text);
			fallVelocity[8] = double.Parse(fallGS8box.Text);
			fallVelocity[9] = double.Parse(fallGS9box.Text);

			input_time_step=int.Parse(input_time_step_box.Text);
            stage_input_time_step = double.Parse(TidalInputStep.Text);
			edgeslope =double.Parse(textBox2.Text);
			lateral_constant=double.Parse(textBox3.Text);
            veg_lat_restriction = double.Parse(veg_lat_box.Text);

            bedrock_erosion_rate = double.Parse(bedrock_erosion_rate_box.Text);
            bedrock_erosion_threshold = double.Parse(bedrock_erosion_threshold_box.Text);

			inpoints=new int[10,2];
            inputpointsarray = new bool[xmax + 2, ymax + 2];

			// then intiailise the main arrays
			if(inbox1.Checked)
			{
				number_of_points++;
				inpoints[0,0]=int.Parse(xbox1.Text);
				inpoints[0,1]=int.Parse(ybox1.Text);
                inputpointsarray[int.Parse(xbox1.Text), int.Parse(ybox1.Text)] = true;
			}

			if(inbox2.Checked)
			{
				number_of_points++;
				inpoints[1,0]=int.Parse(xbox2.Text);
				inpoints[1,1]=int.Parse(ybox2.Text);
                inputpointsarray[int.Parse(xbox2.Text), int.Parse(ybox2.Text)] = true;
			}

			if(inbox3.Checked)
			{
				number_of_points++;
				inpoints[2,0]=int.Parse(xbox3.Text);
				inpoints[2,1]=int.Parse(ybox3.Text);
                inputpointsarray[int.Parse(xbox3.Text), int.Parse(ybox3.Text)] = true;
			}

			if(inbox4.Checked)
			{
				number_of_points++;
				inpoints[3,0]=int.Parse(xbox4.Text);
				inpoints[3,1]=int.Parse(ybox4.Text);
                inputpointsarray[int.Parse(xbox4.Text), int.Parse(ybox4.Text)] = true;
			}

			if(inbox5.Checked)
			{
				number_of_points++;
				inpoints[4,0]=int.Parse(xbox5.Text);
				inpoints[4,1]=int.Parse(ybox5.Text);
                inputpointsarray[int.Parse(xbox5.Text), int.Parse(ybox5.Text)] = true;
			}

			if(inbox6.Checked)
			{
				number_of_points++;
				inpoints[5,0]=int.Parse(xbox6.Text);
				inpoints[5,1]=int.Parse(ybox6.Text);
                inputpointsarray[int.Parse(xbox6.Text), int.Parse(ybox6.Text)] = true;
			}

			if(inbox7.Checked)
			{
				number_of_points++;
				inpoints[6,0]=int.Parse(xbox7.Text);
				inpoints[6,1]=int.Parse(ybox7.Text);
                inputpointsarray[int.Parse(xbox7.Text), int.Parse(ybox7.Text)] = true;
			}

			if(inbox8.Checked)
			{
				number_of_points++;
				inpoints[7,0]=int.Parse(xbox8.Text);
				inpoints[7,1]=int.Parse(ybox8.Text);
                inputpointsarray[int.Parse(xbox8.Text), int.Parse(ybox8.Text)] = true;
			}

			inputfile = new double[number_of_points,(int)((maxcycle*60)/input_time_step)+10,16];
            stage_inputfile = new double[(int)((maxcycle * 60) / stage_input_time_step) + 10];

            elev = new double[xmax+2,ymax+2];
			water_depth = new double[xmax+2,ymax+2];

            old_j_mean_store = new double[(int)((maxcycle*60)/input_time_step)+10];

            qx = new double[xmax + 2, ymax + 2];
            qy = new double[xmax + 2, ymax + 2];

            qxs = new double[xmax + 2, ymax + 2];
            qys = new double[xmax + 2, ymax + 2];


            // temp arrays that control how much sediment goes up,. righ left down and into suspended sediment.
            sr = new Double[xmax + 2, ymax + 2, 10];
            sl = new Double[xmax + 2, ymax + 2, 10];
            su = new Double[xmax + 2, ymax + 2, 10];
            sd = new Double[xmax + 2, ymax + 2, 10];
            ss = new Double[xmax + 2, ymax + 2];

            erodetot = new Double[xmax + 2, ymax + 2];
            erodetot3 = new Double[xmax + 2, ymax + 2];
            temp_elev = new Double[xmax + 2, ymax + 2];


            Vel = new double[xmax + 2, ymax + 2];
			area = new double[xmax+2,ymax+2];
			index = new int[xmax+2,ymax+2];
            elev_diff = new double[xmax + 2, ymax + 2];
            
			bedrock = new double[xmax+2,ymax+2];
			tempcreep = new double[xmax+2,ymax+2];
			init_elevs = new double[xmax+2,ymax+2];
			vel_dir= new double[xmax+2,ymax+2,9];
			
			strata=new double[((xmax+2)*(ymax+2))/LIMIT,10,G_MAX+1];
			grain=new double[((2+xmax)*(ymax+2))/LIMIT,G_MAX+1];

			cross_scan=new int[xmax+2,ymax+2];
			down_scan=new int[ymax+2,xmax+2];
            // time step for rain data
            rain_data_time_step = double.Parse(raintimestepbox.Text);
            mfiletimestep = double.Parse(mfiletimestepbox.Text);

            // line to stop max time step being greater than rain time step
            if (rain_data_time_step < 1) rain_data_time_step = 1;
            if (max_time_step / 60 > rain_data_time_step) max_time_step = (int)rain_data_time_step * 60;

            hourly_rain_data = new double[(int)(maxcycle * (60 / rain_data_time_step)) + 100, rfnum+1];
            hourly_m_value = new double[(int)(maxcycle * (60 / mfiletimestep)) + 10, rfnum + 1];
			climate_data=new double[10001,3];

			temp_grain=new double[G_MAX+1];
			veg=new Double[xmax+1,ymax+1,4]; // 0 is elevation, 1 is level of veg cover (ranging from 0 to 1)
			edge=new double[xmax+1,ymax+1]; // TJC 27/1/05
			edge2=new double[xmax+1,ymax+1]; // TJC 27/4/06
			 
			Tau = new double[xmax+2,ymax+2];
			
            catchment_input_x_coord = new int[xmax * ymax];
            catchment_input_y_coord = new int[xmax * ymax];
            
            //dune things
            dune_mult = (int)(DX)/int.Parse(dune_grid_size_box.Text);
            if (dune_mult < 1) dune_mult = 1;
            if (DuneBox.Checked == false) dune_mult = 1; // needed in order to stop it tripping out the memory
                        
            area_depth = new double[xmax + 2, ymax + 2];
            sand = new double[xmax + 2, ymax + 2];
            elev2 = new double[(xmax * dune_mult) + 2, (ymax*dune_mult) + 2];
            sand2 = new double[(xmax * dune_mult) + 2, (ymax * dune_mult) + 2];
            
            
			dischargeinput=new double[1000,5];
			
			hydrograph=new double[(maxcycle-((int)(cycle/60)))+1000,2];
			

			// MJ arrays 08/02/05

			Vsusptot = new double[xmax+2,ymax+2];

			// JOE arrays 
			slopeAnalysis = new double[xmax+2,ymax+2];		// <JOE 20051605>
			aspect = new double[xmax+2,ymax+2];				// <JOE 20051605>
			hillshade = new double[xmax+2,ymax+2];			// <JOE 20051605>

			//Gez arrays
			sum_grain = new double[G_MAX+1];
            sum_grain2 = new double[G_MAX + 1];
			old_sum_grain = new double[G_MAX+1];
            old_sum_grain2 = new double[G_MAX + 1];
			Qg_step = new double[G_MAX+1];
			Qg_hour = new double[G_MAX+1];
			Qg_over = new double[G_MAX+1];
			Qg_last = new double[G_MAX+1];
            Qg_step2 = new double[G_MAX + 1];
            Qg_hour2 = new double[G_MAX + 1];
            Qg_over2 = new double[G_MAX + 1];
            Qg_last2 = new double[G_MAX + 1];

			CATCH_FILE = TimeseriesOutBox.Text; // MJ 18/01/05
            //CATCH_FILE2 = tracerOutputtextBox.Text;

            // siberia data
            Beta1 = double.Parse(Beta1Box.Text);
            Beta3 = double.Parse(Beta3Box.Text);
            m1 = double.Parse(m1Box.Text);
            m3 = double.Parse(m3Box.Text);
            n1 = double.Parse(n1Box.Text);

            // sediment tpt law
            if (einsteinbox.Checked == true) einstein = 1;
            if (wilcockbox.Checked == true) wilcock = 1;
            if (meyerbox.Checked == true) meyer = 1;

            div_inputs = int.Parse(div_inputs_box.Text);
            recirculate_proportion = double.Parse(propremaining.Text);

            graphics_scale = (int)(1500 / xmax); // auto scales the graphics to be roughly 1000 pixels wide.
            // Create a drawing surface with the same dimensions DEM * graphics scale
            if (graphics_scale < 1) graphics_scale = 1; // catch to prevent too long DEMS causign error..
            m_objDrawingSurface = new Bitmap(xmax * graphics_scale,
               ymax * graphics_scale, System.Drawing.Imaging.PixelFormat.Format24bppRgb);

            // tidal/stagevariables
            fromx = int.Parse(TidalXmin.Text);
            tox = int.Parse(TidalXmax.Text);
            fromy = int.Parse(TidalYmin.Text);
            toy = int.Parse(TidalYmax.Text);

            // soil generation variables
            P1 = double.Parse(textBox11.Text);
            b1 = double.Parse(textBox12.Text);
            k1 = double.Parse(textBox13.Text);
            c1 = double.Parse(textBox14.Text);
            c2 = double.Parse(textBox15.Text);
            k2 = double.Parse(textBox16.Text);
            c3 = double.Parse(textBox17.Text);
            c4 = double.Parse(textBox18.Text);			   

            // disributed hydro model arrays
            j = new Double[rfnum + 1]; //0.000000001; //start of hydrological model paramneters
            jo = new Double[rfnum + 1];//0.000000001;
            j_mean = new Double[rfnum + 1];
            old_j_mean = new Double[rfnum + 1];
            new_j_mean = new Double[rfnum + 1];
            rfarea = new int [xmax + 2, ymax + 2];
            nActualGridCells = new int[rfnum + 1];
            catchment_input_counter = new int[rfnum + 1];
            M = new Double[rfnum + 1];

            // spat var mannings
            spat_var_mannings = new Double[xmax + 1, ymax + 1];

		}


		void sort_active(int x,int y)
		{
			int xyindex;
			double total;
			double amount;
			double coeff;
			int n,z;
			
	
			if (index[x,y] == -9999) addGS(x,y);  // should not be necessary
			xyindex = index[x,y];

			total = 0.0;
			for (n=0;n<=G_MAX;n++)
			{

					total += grain[xyindex,n];

			}

			if (total > (active*1.5)) // depositing - create new strata layer and remove bottom one..
			{
				// start from bottom
				// remove bottom active layer
				// then move all from layer above into one below, up to the top layer
				for(z=9;z>=1;z--)
				{
					for(n=0;n<=G_MAX-2;n++)
					{

							strata[xyindex,z,n]=strata[xyindex,z-1,n];

					}
				}

				// then remove strata thickness from grain - and add to top strata layer
				coeff = active / total;
				for (n=1;n<=(G_MAX-1);n++)
                {
					if ((grain[xyindex,n] > 0.0))
					{

							amount = coeff * (grain[xyindex,n]);
							strata[xyindex,0,n-1] = amount;
							grain[xyindex,n] -= amount;

					}
				}
			}

			if (total < (active/4)) // eroding - eat into existing strata layer & create new one at bottom
			{
				// Start at top
				// Add top strata to grain
				for (n=1;n<=(G_MAX-1);n++)
				{
						grain[xyindex,n] += strata[xyindex,0,n-1];
				}

				// then from top down add lower strata into upper
				for(z=0;z<=8;z++)
				{
					for(n=0;n<=G_MAX-2;n++)
					{
							strata[xyindex,z,n] = strata[xyindex,z+1,n];
					}
				}

				// add new layer at the bottom
				amount = active;
                z = 9;
                for (n = 1; n <= G_MAX - 1; n++)
                {
                    strata[xyindex, z, n - 1] = amount * dprop[n]; // 0.0;

                }
			}

		}

		
        //double layer_depth(int index1, int t)
        //{
			
        //    double total=0;
	
        //    total+=(grain[index1,t,0]);

        //    return (total);
        //}

        //double active_layer_depth(int index1)
        //{
        //    int n;
        //    double active_thickness = 0;

        //    for (n = 1; n <= G_MAX; n++)
        //    {
        //       active_thickness += (grain[index1, n, 0]);
        //    }
        //    return(active_thickness);
        //}


		double sand_fraction(int index1)
		{

			int n;
			double active_thickness=0;
			double sand_total=0;
			for(n=1;n<=G_MAX;n++)
			{
					active_thickness+=(grain[index1,n]);
			}

			for(n=1;n<=2;n++) // number of sand fractions...
			{
					sand_total+=(grain[index1,n]);
			}

			if(active_thickness<0.0001)
			{
				return(0.0);
			}
			else
			{
				return(sand_total/active_thickness);
			}

		}


		double d50(int index1)
		{
			int z,n,i;
			double active_thickness=0;
			double Dfifty=0,max=0,min=0;
			double[] cum_tot;
			cum_tot = new double[20];
            
			for(n=1;n<=G_MAX;n++)
			{
				for(z=0;z<=(0);z++)
				{
					active_thickness+=(grain[index1, n ]);
					cum_tot[n]+=active_thickness;
				}
			}
			

			i=1;
			while(cum_tot[i]<(active_thickness*0.5)&&i<=G_MAX-1)
			{
				i++;
			}
			
			if(i==1){min=Math.Log(d1);max=Math.Log(d1);}
			if(i==2){min=Math.Log(d1);max=Math.Log(d2);}
			if(i==3){min=Math.Log(d2);max=Math.Log(d3);}
			if(i==4){min=Math.Log(d3);max=Math.Log(d4);}
			if(i==5){min=Math.Log(d4);max=Math.Log(d5);}
			if(i==6){min=Math.Log(d5);max=Math.Log(d6);}
			if(i==7){min=Math.Log(d6);max=Math.Log(d7);}
			if(i==8){min=Math.Log(d7);max=Math.Log(d8);}
			if(i==9){min=Math.Log(d8);max=Math.Log(d9);}
			//if(i==9){min=Math.Log(d8);max=Math.Log(d9);}

            Dfifty = Math.Exp(max - ((max - min) * ((cum_tot[i] - (active_thickness * 0.5)) / (cum_tot[i] - cum_tot[i - 1]))));
			if(active_thickness<0.0000001)Dfifty=0;
			return Dfifty;

		}


        double max_bed_slope2(int x, int y)
        {

            double slope = 0;
            int slopetot = 0;
            double slopemax = 0;

            if (elev[x, y] > elev[x, y - 1])
            {
                slope = Math.Pow((elev[x, y] - elev[x, y - 1]) / DX, 1);
                if (slope > slopemax) slopemax = slope;
                slopetot++;
            }
            if (elev[x, y] > elev[x + 1, y - 1])
            {
                slope = Math.Pow((elev[x, y] - elev[x + 1, y - 1]) / root, 1);
                if (slope > slopemax) slopemax = slope;
                slopetot++;
            }
            if (elev[x, y] > elev[x + 1, y])
            {
                slope = Math.Pow((elev[x, y] - elev[x + 1, y]) / DX, 1);
                if (slope > slopemax) slopemax = slope;
                slopetot++;
            }
            if (elev[x, y] > elev[x + 1, y + 1])
            {
                slope = Math.Pow((elev[x, y] - elev[x + 1, y + 1]) / root, 1);
                if (slope > slopemax) slopemax = slope;
                slopetot++;
            }
            if (elev[x, y] > elev[x, y + 1])
            {
                slope = Math.Pow((elev[x, y] - elev[x, y + 1]) / DX, 1);
                if (slope > slopemax) slopemax = slope;
                slopetot++;
            }
            if (elev[x, y] > elev[x - 1, y + 1])
            {
                slope = Math.Pow((elev[x, y] - elev[x - 1, y + 1]) / root, 1);
                if (slope > slopemax) slopemax = slope;
                slopetot++;
            }
            if (elev[x, y] > elev[x - 1, y])
            {
                slope = Math.Pow((elev[x, y] - elev[x - 1, y]) / DX, 1);
                if (slope > slopemax) slopemax = slope;
                slopetot++;
            }
            if (elev[x, y] > elev[x - 1, y - 1])
            {
                slope = Math.Pow((elev[x, y] - elev[x - 1, y - 1]) / root, 1);
                if (slope > slopemax) slopemax = slope;
                slopetot++;
            }
            //if (slope > 0) slope = (slope / slopetot);


            //return(slopemax);
            //if(slopemax<0.001)slopemax=0.001;
            //if (slopemax > 0.01) slopemax = 0.01;
            return (slopemax);
        }


	
		void addGS(int x,int y)
		{
            // needs lock statement to stop two being added at the same time...
            lock (this)
            {
                
                int n, q;
                grain_array_tot++;
                index[x, y] = grain_array_tot;

                grain[grain_array_tot, 0] = 0;
                for (n = 1; n <= G_MAX - 1;n++ )
                {
                    grain[grain_array_tot, n] = active * dprop[n];
                }
                grain[grain_array_tot, G_MAX] = 0;


                for (n = 0; n <= 9; n++)
                {
                    for (int n2 = 0; n2 <= G_MAX - 2;n2++ )
                    {
                        strata[grain_array_tot, n, n2] = (active) * dprop[n2+1];
                    }
                        

                    if (elev[x, y] - (active * (n + 1)) < (bedrock[x, y] - active))
                    {
                        for (q = 0; q <= (G_MAX - 2); q++)
                        {
                            strata[grain_array_tot, n, q] = 0;
                        }
                    }
                }


                sort_active(x, y);

            }

		}

		// calc roughtness on Srickler relationship
		double strickler(int x, int y)
		{
			double temp=0;
			double temp_d50=1;

			if (index[x,y] != -9999)temp_d50=d50(index[x,y]); 
			temp =0.034*Math.Pow(temp_d50,0.167);
			//Console.WriteLine(Convert.ToString(strickler(x,y)));
			temp+=0.012;
			return temp;

		}


        ////Calculate roughness - Added JMW 20051122
        ////Based on Baptist, 2005
        //double chezy(int x, int y)
        //{
        //    double Cr;
        //    double Cb;
        //    double temp_d50=30;

        //    double temp_depth=water_depth[x,y];
			
        //    if(temp_depth<=0)temp_depth=0.1;
			

        //    // just to save it when there is no grain file
        //    if (index[x,y] != -9999)temp_d50=d50(index[x,y]); 
			
            

        //    // Grain roughness (strickler formulation)
        //    Cb = (26.4*Math.Pow(temp_depth/temp_d50,0.167));
        //    Cr=Cb;

        //    // veg influencing roughness is commented out at the moment...
        //    //if (veg[x,y,1]>0.01)// if there is any veg..
        //    //{
        //    //    // Applying vegetation
        //    //    if (veg[x,y,3] < temp_depth)
        //    //    {
        //    //        Cr = (Math.Pow(1/(1/(Cb*Cb) + veg[x,y,2]*veg[x,y,3]/(2*g)),0.5)
        //    //            + Math.Pow(g,0.5)/kappa*Math.Log(temp_depth/veg[x,y,3]));
        //    //    }
        //    //    else
        //    //    {                 
        //    //        Cr = (Math.Pow(1/(1/(Cb*Cb) + veg[x,y,2]*temp_depth/(2*g)),0.5));
        //    //    }
        //    //}

        //    return Cr;
        //}


        //double chezytomanning(double chezyc, double depth)
        //{
        //    return (Math.Pow(depth,0.167))/chezyc;
        //}


        //double calc_manning(int x, int y)
        //{
        //    double temp_depth = water_depth[x, y];
        //    if (temp_depth <= 0) temp_depth = 0.1;
        //    return chezytomanning(chezy(x, y), temp_depth) + 0.012;
        //    return 0.03 + (veg[x, y, 2] * 0.03);
        //    if (veg[x, y, 1] > 0.5)
        //    {
        //        return 0.05;
        //    }
        //    else
        //    {
        //        return 0.03;
        //    }
        //}


		void grow_grass(double amount3)
		{

            int x, y;
			for(x=1;x<=xmax;x++)
			{
				for(y=1;y<=ymax;y++)
				{
                    //first check if veg is at 0.. not sure if this is needed now..
                    if (veg[x, y, 0] == 0)
                    {
                        veg[x, y, 0] = elev[x, y];
                    }

					// first check if under water or not..
					if(water_depth[x,y]<water_depth_erosion_threshold)
					{
                        // if not then it
                        // now adds to the amount of veg there.. 
						veg[x,y,1]+=amount3;
						if(veg[x,y,1]>1)veg[x,y,1]=1;
					}

                    // check if veg below elev! if so raise to elev
                    if (veg[x, y, 0] < elev[x, y]) // raises the veg level if it gets buried.. but only by a certain amount (0.001m day...)
                    {
                        veg[x, y, 0] += 0.001; // this is an arbitrary amount..
                        if (veg[x, y, 0] > elev[x, y]) veg[x, y, 0] = elev[x, y];
                    }

                    // check if veg above elev! if so lower to elev
                    if (veg[x, y, 0] > elev[x, y]) 
                    {
                        veg[x, y, 0] -= 0.001; // this is an arbitrary amount..
                        if (veg[x, y, 0] < elev[x, y]) veg[x, y, 0] = elev[x, y];
                    }

					// trying to see now, if veg is above elev - so if lateral erosion happens - more than 0.1m the veg gets wiped..

                    if (veg[x, y, 0] - elev[x, y] > 0.1)
                    {
                        veg[x, y, 1] = 0;
                        veg[x, y, 0] = elev[x, y];
                    }


					// now see if veg under water - if so let it die back a bit..
                    if(water_depth[x,y] > water_depth_erosion_threshold&&veg[x,y,1]>0)
                    {
                        veg[x, y, 1] -= (amount3 / 2);
                        if (veg[x, y, 1] < 0)
                        {
                            veg[x, y, 1] = 0;
                            veg[x, y, 0] = elev[x, y]; // resets elev if veg amt is 0
                        }
                    }

                    // also if it is under sediment - then dies back a bit too...
                    if (veg[x, y, 0] < elev[x, y]) 
                    {
                        veg[x, y, 1] -= (amount3 / 2);
                        if (veg[x, y, 1] < 0)
                        {
                            veg[x, y, 1] = 0;
                            veg[x, y, 0] = elev[x, y]; // resets elev if veg amt is 0
                        }
                        
                    }

                    // but if it is under sediment, has died back to nearly 0 (0.05) then it resets the elevation 
                    // to the surface elev.
                    if (veg[x, y, 0] < elev[x, y]&&veg[x,y,1]<0.05)
                    {
                        veg[x, y, 0] = elev[x, y];
                    }


				}
			}


		}


		void creep(double time)
		{

			/** creep rate is 10*-2 * slope per year, so inputs time jump in years*/
			/** very important differnece here is that slide_GS() is called only if
				BOTH cells are not -9999 therfore if both have grainsize then do additions.
				this is to stop the progressive spread of selected cells upslope */

		

			int x,y;
			double temp;


			for(x=1;x<=xmax;x++)
			{
				for(y=1;y<=ymax;y++)
				{
					tempcreep[x,y]=0;
				}
			}

	
			for(x=2;x<xmax;x++)
			{
				for(y=2;y<ymax;y++)
				{
					if(elev[x,y]>bedrock[x,y])
					{
						if(elev[x,y-1]<elev[x,y]&&elev[x,y-1]> -9999)
						{
							temp=((elev[x,y]-elev[x,y-1])/ DX)*CREEP_RATE*time/ DX;
							if((elev[x,y]-temp)<bedrock[x,y])temp=elev[x,y]-bedrock[x,y];
							if(temp<0)temp=0;				
							tempcreep[x,y]-=temp;
							tempcreep[x,y-1]+=temp;
							if(index[x,y-1]!=-9999)
							{
                                slide_GS(x, y, temp, x, y - 1);
							}
						}
						if(elev[x+1,y-1]<elev[x,y]&&elev[x+1,y-1]> -9999)
						{
							temp=((elev[x,y]-elev[x+1,y-1])/ root)*CREEP_RATE*time/ root;
							if((elev[x,y]-temp)<bedrock[x,y])temp=elev[x,y]-bedrock[x,y];
							if(temp<0)temp=0;
							tempcreep[x,y]-=temp;
							tempcreep[x+1,y-1]+=temp;
							if(index[x+1,y-1]!=-9999)
							{
                                slide_GS(x, y, temp, x+1, y - 1);
							}
			
						}
						if(elev[x+1,y]<elev[x,y]&&elev[x+1,y]> -9999)
						{
							temp=((elev[x,y]-elev[x+1,y])/ DX)*CREEP_RATE*time/ DX;
							if((elev[x,y]-temp)<bedrock[x,y])temp=elev[x,y]-bedrock[x,y];
							if(temp<0)temp=0;
							tempcreep[x,y]-=temp;
							tempcreep[x+1,y]+=temp;
							if(index[x+1,y]!=-9999)
							{
                                slide_GS(x, y, temp, x+1, y);
							}
						}
						if(elev[x+1,y+1]<elev[x,y]&&elev[x+1,y+1]> -9999)
						{
							temp=((elev[x,y]-elev[x+1,y+1])/ root)*CREEP_RATE*time/ root;
							if((elev[x,y]-temp)<bedrock[x,y])temp=elev[x,y]-bedrock[x,y];
							if(temp<0)temp=0;
							tempcreep[x,y]-=temp;
							tempcreep[x+1,y+1]+=temp;
							if(index[x+1,y+1]!=-9999)
							{
                                slide_GS(x, y, temp, x+1, y + 1);
							}
						}
						if(elev[x,y+1]<elev[x,y]&&elev[x,y+1]> -9999)
						{
							temp=((elev[x,y]-elev[x,y+1])/ DX)*CREEP_RATE*time/ DX;
							if((elev[x,y]-temp)<bedrock[x,y])temp=elev[x,y]-bedrock[x,y];
							if(temp<0)temp=0;
							tempcreep[x,y]-=temp;
							tempcreep[x,y+1]+=temp;
							if(index[x,y+1]!=-9999)
							{
                                slide_GS(x, y, temp, x, y + 1);
							}
						}
						if(elev[x-1,y+1]<elev[x,y]&&elev[x-1,y+1]> -9999)
						{
							temp=((elev[x,y]-elev[x-1,y+1])/ root)*CREEP_RATE*time/ root;
							if((elev[x,y]-temp)<bedrock[x,y])temp=elev[x,y]-bedrock[x,y];
							if(temp<0)temp=0;
							tempcreep[x,y]-=temp;
							tempcreep[x-1,y+1]+=temp;
							if(index[x-1,y+1]!=-9999)
							{
                                slide_GS(x, y, temp, x-1, y + 1);
							}
						}
						if(elev[x-1,y]<elev[x,y]&&elev[x-1,y]> -9999)
						{
							temp=((elev[x,y]-elev[x-1,y])/ DX)*CREEP_RATE*time/ DX;
							if((elev[x,y]-temp)<bedrock[x,y])temp=elev[x,y]-bedrock[x,y];
							if(temp<0)temp=0;
							tempcreep[x,y]-=temp;
							tempcreep[x-1,y]+=temp;
							if(index[x-1,y]!=-9999)
							{
                                slide_GS(x, y, temp, x-1, y );
							}
						}
						if(elev[x-1,y-1]<elev[x,y]&&elev[x-1,y-1]> -9999)
						{
							temp=((elev[x,y]-elev[x-1,y-1])/ root)*CREEP_RATE*time/ root;
							if((elev[x,y]-temp)<bedrock[x,y])temp=elev[x,y]-bedrock[x,y];
							if(temp<0)temp=0;
							tempcreep[x,y]-=temp;
							tempcreep[x-1,y-1]+=temp;
							if(index[x-1,y-1]!=-9999)
							{
                                slide_GS(x, y, temp, x-1, y - 1);
							}
						}
					}
				}
			}
	
			for(x=1;x<=xmax;x++)
			{
				for(y=1;y<=ymax;y++)
				{
					elev[x,y]+=tempcreep[x,y];
				}
			}

		}

        void siberia(double time)
        {
            int x, y;
            double temp;
            double SibQ = 0;
            double SibQsf = 0;

                        

            for (x = 1; x <= xmax; x++)
            {
                for (y = 1; y <= ymax; y++)
                {
                    tempcreep[x, y] = 0;
                }
            }


            for (x = 2; x < xmax; x++)
            {
                for (y = 2; y < ymax; y++)
                {
                    if (elev[x, y] > bedrock[x, y])
                    {
                        //SibQ =  Math.Pow(area[x, y] * DX * DX, m3);
                        //SibQsf = DX * Beta3 * Beta1 * Math.Pow((SibQ), m1) * Math.Pow(max_bed_slope2(x, y), n1) * time;

                        SibQ = Beta3 * Math.Pow(area[x, y] * DX * DX, m3);
                        SibQsf = Beta1 * Math.Pow((SibQ / DX), m1) * Math.Pow(max_bed_slope2(x, y), n1) * time;
                        double max_slope = max_bed_slope2(x, y);
                        if (max_slope == 0) max_slope = 10; // catch line to stop erosion if there is no slope
                        //if (max_slope > 0) max_slope *= 0.99;

                        if ((elev[x, y]-elev[x, y - 1] )/ DX >= max_slope && elev[x, y - 1] > -9999)
                        {
                            temp = SibQsf;
                            if ((elev[x, y] - temp) < bedrock[x, y]) temp = elev[x, y] - bedrock[x, y];
                            if (temp < 0) temp = 0;
                            tempcreep[x, y] -= temp;
                            tempcreep[x, y - 1] += temp;
                            if (index[x, y] != -9999)
                            {
                                slide_GS(x, y, temp, x, y - 1);
                            }
                        }
                        if ((elev[x, y] - elev[x + 1, y - 1]) / root >= max_slope && (elev[x + 1, y - 1] > -9999 || x == xmax))
                        {
                            temp = SibQsf;
                            if ((elev[x, y] - temp) < bedrock[x, y]) temp = elev[x, y] - bedrock[x, y];
                            if (temp < 0) temp = 0;
                            tempcreep[x, y] -= temp;
                            tempcreep[x + 1, y - 1] += temp;
                            if (index[x, y] != -9999)
                            {
                                slide_GS(x, y, temp, x + 1, y - 1);
                            }

                        }
                        if ((elev[x, y] - elev[x + 1, y]) / DX >= max_slope && (elev[x + 1, y] > -9999 || x == xmax))
                        {
                            temp = SibQsf;
                            if ((elev[x, y] - temp) < bedrock[x, y]) temp = elev[x, y] - bedrock[x, y];
                            if (temp < 0) temp = 0;
                            tempcreep[x, y] -= temp;
                            tempcreep[x + 1, y] += temp;
                            if (index[x, y] != -9999)
                            {
                                slide_GS(x, y, temp, x + 1, y);
                            }
                        }
                        if ((elev[x, y] - elev[x + 1, y + 1]) / root >= max_slope && (elev[x + 1, y + 1] > -9999 || x == xmax))
                        {
                            temp = SibQsf;
                            if ((elev[x, y] - temp) < bedrock[x, y]) temp = elev[x, y] - bedrock[x, y];
                            if (temp < 0) temp = 0;
                            tempcreep[x, y] -= temp;
                            tempcreep[x + 1, y + 1] += temp;
                            if (index[x, y] != -9999)
                            {
                                slide_GS(x, y, temp, x + 1, y + 1);
                            }
                        }
                        if ((elev[x, y] - elev[x, y + 1]) / DX >= max_slope && elev[x, y + 1] > -9999 )
                        {
                            temp = SibQsf;
                            if ((elev[x, y] - temp) < bedrock[x, y]) temp = elev[x, y] - bedrock[x, y];
                            if (temp < 0) temp = 0;
                            tempcreep[x, y] -= temp;
                            tempcreep[x, y + 1] += temp;
                            if (index[x, y] != -9999)
                            {
                                slide_GS(x, y, temp, x, y + 1);
                            }
                        }
                        if ((elev[x, y] - elev[x - 1, y + 1]) / root >= max_slope && elev[x - 1, y + 1] > -9999 )
                        {
                            temp = SibQsf;
                            if ((elev[x, y] - temp) < bedrock[x, y]) temp = elev[x, y] - bedrock[x, y];
                            if (temp < 0) temp = 0;
                            tempcreep[x, y] -= temp;
                            tempcreep[x - 1, y + 1] += temp;
                            if (index[x, y] != -9999)
                            {
                                slide_GS(x, y, temp, x - 1, y + 1);
                            }
                        }
                        if ((elev[x, y] - elev[x - 1, y]) / DX >= max_slope && elev[x - 1, y] > -9999 )
                        {
                            temp = SibQsf;
                            if ((elev[x, y] - temp) < bedrock[x, y]) temp = elev[x, y] - bedrock[x, y];
                            if (temp < 0) temp = 0;
                            tempcreep[x, y] -= temp;
                            tempcreep[x - 1, y] += temp;
                            if (index[x, y] != -9999)
                            {
                                slide_GS(x, y, temp, x - 1, y);
                            }
                        }
                        if ((elev[x, y] - elev[x - 1, y - 1]) / root >= max_slope && elev[x - 1, y - 1] > -9999 )
                        {
                            temp = SibQsf;
                            if ((elev[x, y] - temp) < bedrock[x, y]) temp = elev[x, y] - bedrock[x, y];
                            if (temp < 0) temp = 0;
                            tempcreep[x, y] -= temp;
                            tempcreep[x - 1, y - 1] += temp;
                            if (index[x, y] != -9999)
                            {
                                slide_GS(x, y, temp, x - 1, y - 1);
                            }
                        }

                    }
                }
            }

            for (x = 2; x < xmax; x++)
            {
                for (y = 2; y < ymax; y++)
                {
                    elev[x, y] += tempcreep[x, y];
                }
            }

        }


        void soilerosion(double time)
        {

            /** creep rate is 10*-2 * slope per year, so inputs time jump in years*/
            /** very important differnece here is that slide_GS() is called only if
                BOTH cells are not -9999 therfore if both have grainsize then do additions.
                this is to stop the progressive spread of selected cells upslope */



            int x, y;
            double temp;


            for (x = 1; x <= xmax; x++)
            {
                for (y = 1; y <= ymax; y++)
                {
                    tempcreep[x, y] = 0;
                }
            }


            for (x = 2; x < xmax; x++)
            {
                for (y = 2; y < ymax; y++)
                {
                    if (elev[x, y] > bedrock[x, y])
                    {
                        if (elev[x, y - 1] < elev[x, y] && elev[x, y - 1] > -9999)
                        {
                            temp = ((elev[x, y] - elev[x, y - 1]) / DX) * SOIL_RATE * time / DX * Math.Pow(area[x,y]*DX*DX,0.5);
                            if ((elev[x, y] - temp) < bedrock[x, y]) temp = elev[x, y] - bedrock[x, y];
                            if (temp < 0) temp = 0;
                            tempcreep[x, y] -= temp;
                            tempcreep[x, y - 1] += temp;
                            if (index[x, y] != -9999)
                            {
                                slide_GS(x, y, temp, x, y - 1);
                            }
                        }
                        if (elev[x + 1, y - 1] < elev[x, y] && elev[x + 1, y - 1] > -9999)
                        {
                            temp = ((elev[x, y] - elev[x + 1, y - 1]) / root) * SOIL_RATE * time / root * Math.Pow(area[x, y] * DX * DX, 0.5);
                            if ((elev[x, y] - temp) < bedrock[x, y]) temp = elev[x, y] - bedrock[x, y];
                            if (temp < 0) temp = 0;
                            tempcreep[x, y] -= temp;
                            tempcreep[x + 1, y - 1] += temp;
                            if (index[x, y] != -9999)
                            {
                                slide_GS(x, y, temp, x + 1, y - 1);
                            }

                        }
                        if (elev[x + 1, y] < elev[x, y] && elev[x + 1, y] > -9999)
                        {
                            temp = ((elev[x, y] - elev[x + 1, y]) / DX) * SOIL_RATE * time / DX * Math.Pow(area[x, y] * DX * DX, 0.5);
                            if ((elev[x, y] - temp) < bedrock[x, y]) temp = elev[x, y] - bedrock[x, y];
                            if (temp < 0) temp = 0;
                            tempcreep[x, y] -= temp;
                            tempcreep[x + 1, y] += temp;
                            if (index[x, y] != -9999)
                            {
                                slide_GS(x, y, temp, x + 1, y);
                            }
                        }
                        if (elev[x + 1, y + 1] < elev[x, y] && elev[x + 1, y + 1] > -9999)
                        {
                            temp = ((elev[x, y] - elev[x + 1, y + 1]) / root) * SOIL_RATE * time / root * Math.Pow(area[x, y] * DX * DX, 0.5);
                            if ((elev[x, y] - temp) < bedrock[x, y]) temp = elev[x, y] - bedrock[x, y];
                            if (temp < 0) temp = 0;
                            tempcreep[x, y] -= temp;
                            tempcreep[x + 1, y + 1] += temp;
                            if (index[x, y] != -9999)
                            {
                                slide_GS(x, y, temp, x + 1, y + 1);
                            }
                        }
                        if (elev[x, y + 1] < elev[x, y] && elev[x, y + 1] > -9999)
                        {
                            temp = ((elev[x, y] - elev[x, y + 1]) / DX) * SOIL_RATE * time / DX * Math.Pow(area[x, y] * DX * DX, 0.5);
                            if ((elev[x, y] - temp) < bedrock[x, y]) temp = elev[x, y] - bedrock[x, y];
                            if (temp < 0) temp = 0;
                            tempcreep[x, y] -= temp;
                            tempcreep[x, y + 1] += temp;
                            if (index[x, y] != -9999)
                            {
                                slide_GS(x, y, temp, x, y + 1);
                            }
                        }
                        if (elev[x - 1, y + 1] < elev[x, y] && elev[x - 1, y + 1] > -9999)
                        {
                            temp = ((elev[x, y] - elev[x - 1, y + 1]) / root) * SOIL_RATE * time / root * Math.Pow(area[x, y] * DX * DX, 0.5);
                            if ((elev[x, y] - temp) < bedrock[x, y]) temp = elev[x, y] - bedrock[x, y];
                            if (temp < 0) temp = 0;
                            tempcreep[x, y] -= temp;
                            tempcreep[x - 1, y + 1] += temp;
                            if (index[x, y] != -9999)
                            {
                                slide_GS(x, y, temp, x - 1, y + 1);
                            }
                        }
                        if (elev[x - 1, y] < elev[x, y] && elev[x - 1, y] > -9999)
                        {
                            temp = ((elev[x, y] - elev[x - 1, y]) / DX) * SOIL_RATE * time / DX * Math.Pow(area[x, y] * DX * DX, 0.5);
                            if ((elev[x, y] - temp) < bedrock[x, y]) temp = elev[x, y] - bedrock[x, y];
                            if (temp < 0) temp = 0;
                            tempcreep[x, y] -= temp;
                            tempcreep[x - 1, y] += temp;
                            if (index[x, y] != -9999)
                            {
                                slide_GS(x, y, temp, x - 1, y);
                            }
                        }
                        if (elev[x - 1, y - 1] < elev[x, y] && elev[x - 1, y - 1] > -9999)
                        {
                            temp = ((elev[x, y] - elev[x - 1, y - 1]) / root) * SOIL_RATE * time / root * Math.Pow(area[x, y] * DX * DX, 0.5);
                            if ((elev[x, y] - temp) < bedrock[x, y]) temp = elev[x, y] - bedrock[x, y];
                            if (temp < 0) temp = 0;
                            tempcreep[x, y] -= temp;
                            tempcreep[x - 1, y - 1] += temp;
                            if (index[x, y] != -9999)
                            {
                                slide_GS(x, y, temp, x - 1, y - 1);
                            }
                        }
                    }
                }
            }

            for (x = 1; x <= xmax; x++)
            {
                for (y = 1; y <= ymax; y++)
                {
                    elev[x, y] += tempcreep[x, y];
                }
            }

        }


		void get_area()
		{
			int x,y;

			for(x=1;x<=xmax;x++)
			{
				for(y=1;y<=ymax;y++)
				{
					area_depth[x,y]=1;
                    area[x, y] = 0;
                    if (elev[x, y] == -9999)
                    {
                        area_depth[x, y] = 0.0;
                    }
					
				}
			}
			get_area4();
  
		}


        void get_area4()
        {

            // new routine for determining drainage area 4/10/2010
            // instead of using sweeps this sorts all the elevations then works frmo the
            // highest to lowest - calculating drainage area - D-infinity basically.

            int x, y, n, x2,y2, dir;

            // zero load of arrays
            double [] tempvalues,tempvalues2, xkey, ykey;
            tempvalues = new Double [(xmax+2) *(ymax+2)];
            tempvalues2 = new Double[(xmax+2) * (ymax+2)];
            xkey = new Double [(xmax+2)*(ymax+2)];
            ykey = new Double[(xmax+2) * (ymax+2)];

            // then createst temp array based on elevs then also one for x values. 
            int inc = 1;
            for (y = 1; y <= ymax; y++)
            {
                for (x = 1; x <= xmax; x++)    
                {
                    tempvalues[inc] = elev[x, y];
                    xkey[inc] = x;
                    inc++;
                }
            }
            // then sorts according to elevations - but also sorts the key (xkey) according to these too..
            Array.Sort(tempvalues, xkey);

            // now does the same for y values
            inc = 1;
            for (y = 1; y <= ymax; y++)
            {
                for (x = 1; x <= xmax; x++)
                {
                    tempvalues2[inc] = elev[x, y];
                    ykey[inc] = y;
                    inc++;
                }
            }

            Array.Sort(tempvalues2, ykey); 


            // then works through the list of x and y co-ordinates from highest to lowest...
            for (n = (xmax * ymax); n >= 1; n--)
            {
                x = (int)(xkey[n]);
                //this.InfoStatusPanel.Text = Convert.ToString(x);
                y = (int)(ykey[n]);
                //this.InfoStatusPanel.Text = Convert.ToString(y);

                if (area_depth[x, y] > 0)
                {
                    // update area if area_depth is higher
                    if (area_depth[x, y] > area[x, y]) area[x, y] = area_depth[x, y];

                    double difftot = 0;

                    // work out sum of +ve slopes in all 8 directions
                    for (dir = 1; dir <= 8; dir++)//was 1 to 8 +=2
                    {

                        x2 = x + deltaX[dir];
                        y2 = y + deltaY[dir];
                        if (x2 < 1) x2 = 1; if (y2 < 1) y2 = 1; if (x2 > xmax) x2 = xmax;if(y2>ymax)y2=ymax;

                        // swap comment lines below for drainage area from D8 or Dinfinity
                        if (Math.IEEERemainder(dir, 2) != 0)
                        {
                            if (elev[x2, y2] < elev[x, y]) difftot += elev[x, y] - elev[x2, y2];
                        }
                        else
                        {
                            if (elev[x2, y2] < elev[x, y]) difftot += (elev[x, y] - elev[x2, y2]) / 1.414;
                        }
                        //if(elev[x,y]-elev[x2,y2]>difftot)difftot=elev[x,y]-elev[x2,y2];
                    }
                    if (difftot > 0)
                    {
                        // then distribute to all 8...
                        for (dir = 1; dir <= 8; dir++)//was 1 to 8 +=2
                        {

                            x2 = x + deltaX[dir];
                            y2 = y + deltaY[dir];
                            if (x2 < 1) x2 = 1; if (y2 < 1) y2 = 1; if (x2 > xmax) x2 = xmax; if (y2 > ymax) y2 = ymax;

                            // swap comment lines below for drainage area from D8 or Dinfinity
                            
                            if (Math.IEEERemainder(dir, 2) != 0)
                            {
                                if (elev[x2, y2] < elev[x, y]) area_depth[x2, y2] += area_depth[x, y] * ((elev[x, y] - elev[x2, y2]) / difftot);
                            }
                            else
                            {
                                if (elev[x2, y2] < elev[x, y]) area_depth[x2, y2] += area_depth[x, y] * (((elev[x, y] - elev[x2, y2])/1.414) / difftot);
                            }
                            
                            //if (elev[x, y] - elev[x2, y2] == difftot) area_depth[x2, y2] += area_depth[x, y];
                        }

                    }
                    // finally zero the area depth...
                    area_depth[x, y] = 0;
                }
            }

        }


        void init_route(int flag, double reach_input_amount, double catchment_input_amount)
		{
			int x,y,inc;
			
			double w = water_depth_erosion_threshold;


			/*******************************/
	
			for(x=1;x<=xmax;x++)
			{
				for(y=1;y<=ymax;y++)
				{
					cross_scan[x,y]=0;
					down_scan[y,x]=0;
					//if(water_depth[x,y]==-9999)water_depth[x,y]=0;
				}
			}
			
	

			for(x=1;x<=xmax;x++)
			{
				inc=1;
				for(y=1;y<=ymax;y++)
				{
                    if (water_depth[x, y] > 0
                        || water_depth[x - 1, y] > 0
                        || water_depth[x + 1, y] > 0
                        || water_depth[x, y - 1] > 0
                        || water_depth[x, y + 1] > 0)
					{
						cross_scan[x,inc]=y;
						inc++;
					}	
					//discharge[x,y]=0;			
				}
			}	

			for(y=1;y<=ymax;y++)
			{
				inc=1;
				for(x=xmax;x>=1;x--)
				{
					if(water_depth[x,y]>0
                        ||water_depth[x-1,y]>0
                        ||water_depth[x+1,y]>0
                        ||water_depth[x,y-1]>0
                        ||water_depth[x,y+1]>0)
					{
						down_scan[y,inc]=x;
						inc++;
					}
					//if(input_type_flag==1)discharge[x,y]=0;
				}
			}
	

			this.InfoStatusPanel.Text="Running";		// MJ 14/01/05

		}

        //double Fi(int index1, int t)
        //{
        //    int n;
        //    double active_thickness = 0;
        //    double total = 0;

        //    // part to deal with tracers is now hard coded in order to speed things up - with
        //    // an if statement at the start.. saves un-necessary looping


        //    for (n = 1; n <= G_MAX; n++)
        //    {
        //        active_thickness += (grain[index1, n]);
        //    }


        //    total += (grain[index1, t]);

        //    if (active_thickness < 0.0001)
        //    {
        //        return (0.0);
        //    }
        //    else
        //    {
        //        return (total / active_thickness);
        //    }
        //}
        		
		private void calc_J(double cycle)
		{
            for (int n = 1; n <= rfnum; n++)
            {
                double local_rain_fall_rate; /** in m/second **/
                double local_time_step = 60; /** in secs */


                old_j_mean[n] = new_j_mean[n];
                jo[n] = j[n];

                /* Get The M Value From File If One Is Specified */
                if (variable_m_value_flag == 1)
                {
                    M[n] = hourly_m_value[1 + (int)(cycle / mfiletimestep), n];
                }

                local_rain_fall_rate = 0;
                if (hourly_rain_data[(int)(cycle / rain_data_time_step), n] > 0)
                {
                    local_rain_fall_rate = rain_factor * ((hourly_rain_data[(int)(cycle / rain_data_time_step), n] / 1000) / 3600); /** divide by 1000 to make m/hr, then by 3600 for m/sec */
                }

                if (local_rain_fall_rate == 0)
                {
                    j[n] = jo[n] / (1 + ((jo[n] * local_time_step) / M[n]));
                    new_j_mean[n] = M[n] / local_time_step * Math.Log(1 + ((jo[n] * local_time_step) / M[n]));
                }

                if (local_rain_fall_rate > 0)
                {
                    j[n] = local_rain_fall_rate / (((local_rain_fall_rate - jo[n]) / jo[n]) * Math.Exp((0 - local_rain_fall_rate) * local_time_step / M[n]) + 1);
                    new_j_mean[n] = (M[n] / local_time_step) * Math.Log(((local_rain_fall_rate - jo[n]) + jo[n] * Math.Exp((local_rain_fall_rate * local_time_step) / M[n])) / local_rain_fall_rate);
                }
                if (new_j_mean[n] < 0) new_j_mean[n] = 0;
                /**printf("new J mean = %f\n",(new_j_mean*1000));*/
            }

		}

        double calc_max_flow_direction_degrees(int x, int y)
        {
            double deg = 0;
            if (water_depth[x, y] > water_depth_erosion_threshold)
            {
                double xplus = vel_dir[x, y, 3];
                double xminus = vel_dir[x, y, 7];
                double yplus = vel_dir[x, y, 5];
                double yminus = vel_dir[x, y, 1];
                if (xplus < 0) xplus = 0;
                if (xminus < 0) xminus = 0;
                if (yplus < 0) yplus = 0;
                if (yminus < 0) yminus = 0;
                double xbalance = xplus - xminus;
                double ybalance = yminus - yplus;

                //working out max_flow for 12-3pm

                if (xbalance > 0 && ybalance < 0)
                {

                    deg = (Math.Atan(xbalance / Math.Abs(ybalance)) * (180 / 3.142));

                }

                // for 3pm to 6pm
                if (xbalance > 0 && ybalance > 0)
                {
                    deg = 90 + (Math.Atan(ybalance / xbalance) * (180 / 3.142));
                }

                // for 6 - 9 
                if (xbalance < 0 && ybalance > 0)
                {
                    deg = 180 + (Math.Atan(Math.Abs(xbalance) / Math.Abs(ybalance)) * (180 / 3.142));
                }

                // for 9-12
                if (xbalance < 0 && ybalance < 0)
                {
                    deg = 270 + (Math.Atan(Math.Abs(ybalance) / Math.Abs(xbalance)) * (180 / 3.142));
                }

                if (xbalance > 0 && ybalance == 0) deg = 90;
                if (xbalance < 0 && ybalance == 0) deg = 270;
                if (ybalance > 0 && xbalance == 0) deg = 180;
                if (ybalance < 0 && xbalance == 0) deg = 0;

                return (deg);

            }
            else
            {
                return -9999.0;
            }


        }

        void qroute()
        {
            double local_time_factor = time_factor;
            if (local_time_factor > (courant_number * (DX / Math.Sqrt(gravity * (maxdepth))))) local_time_factor = courant_number * (DX / Math.Sqrt(gravity * (maxdepth)));

            var options = new ParallelOptions { MaxDegreeOfParallelism = Environment.ProcessorCount *  4 };
            Parallel.For(1, ymax+1, options, delegate(int y)
            {
                int inc = 1;
                while (down_scan[y, inc] > 0)
                {
                    int x = down_scan[y, inc];
                    inc++;

                    if (elev[x, y] > -9999) // to stop moving water in to -9999's on elev
                    {
                        // add spatial mannings here
                        double temp_mannings = mannings;
                        if (SpatVarManningsCheckbox.Checked == true) temp_mannings = spat_var_mannings[x, y];
                        
                        // routing in x direction
                        if ((water_depth[x, y] > 0 || water_depth[x - 1, y] > 0) && elev[x - 1, y] > -9999)  // need to check water and not -9999 on elev
                        {
                            double hflow = Math.Max(elev[x, y] + water_depth[x, y], elev[x - 1, y] + water_depth[x - 1, y]) -
                                            Math.Max(elev[x - 1, y], elev[x, y]);

                            if (hflow > hflow_threshold)
                            {
                                double tempslope = (((elev[x - 1, y] + water_depth[x - 1, y])) -
                                        (elev[x, y] + water_depth[x, y])) / DX;

                                if (x == xmax) tempslope = edgeslope;
                                if (x <= 2) tempslope = 0 - edgeslope;

                                //double oldqx = qx[x, y];
                                qx[x, y] = ((qx[x, y] - (gravity * hflow * local_time_factor * tempslope)) /
                                        (1 + gravity * hflow * local_time_factor * (temp_mannings * temp_mannings) * Math.Abs(qx[x, y]) /
                                        Math.Pow(hflow, (10 / 3))));
                                //if (oldqx != 0) qx[x, y] = (oldqx + qx[x, y]) / 2;

                                // need to have these lines to stop too much water moving from one cellt o another - resulting in -ve discharges
                                // whihc causes a large instability to develop - only in steep catchments really
                                if (qx[x, y] > 0 && (qx[x, y] / hflow)/Math.Sqrt(gravity*hflow) > froude_limit ) qx[x, y] = hflow * (Math.Sqrt(gravity*hflow) * froude_limit );
                                if (qx[x, y] < 0 && Math.Abs(qx[x, y] / hflow) / Math.Sqrt(gravity * hflow) > froude_limit ) qx[x, y] = 0 - (hflow * (Math.Sqrt(gravity * hflow) * froude_limit ));
                                
                                if (qx[x, y] > 0 && (qx[x, y] * local_time_factor / DX) > (water_depth[x, y] / 4)) qx[x, y] = ((water_depth[x, y] * DX) / 5) / local_time_factor;
                                if (qx[x, y] < 0 && Math.Abs(qx[x, y] * local_time_factor / DX) > (water_depth[x - 1, y] / 4)) qx[x, y] = 0 - ((water_depth[x - 1, y] * DX) / 5) / local_time_factor;

                                if (isSuspended[1])
                                {

                                    if (qx[x, y] > 0) qxs[x, y] = qx[x, y] * (Vsusptot[x, y] / water_depth[x, y]);
                                    if (qx[x, y] < 0) qxs[x, y] = qx[x, y] * (Vsusptot[x - 1, y] / water_depth[x - 1, y]);

                                    if (qxs[x, y] > 0 && qxs[x, y] * local_time_factor > (Vsusptot[x, y] * DX) / 4) qxs[x, y] = ((Vsusptot[x, y] * DX) / 5) / local_time_factor;
                                    if (qxs[x, y] < 0 && Math.Abs(qxs[x, y] * local_time_factor) > (Vsusptot[x - 1, y] * DX) / 4) qxs[x, y] = 0 - ((Vsusptot[x - 1, y] * DX) / 5) / local_time_factor;
                                }

                                // calc velocity now
                                if (qx[x, y] > 0) vel_dir[x, y, 7] = qx[x, y] / hflow;
                                if (qx[x, y] < 0) vel_dir[x - 1, y, 3] = (0- qx[x, y]) / hflow;

                            }
                            else
                            {
                                qx[x, y] = 0;
                                qxs[x, y] = 0;
                            }
                        }

                        //routing in the y direction
                        if ((water_depth[x, y] > 0 || water_depth[x, y - 1] > 0) && elev[x, y - 1] > -9999)
                        {
                            double hflow = Math.Max(elev[x, y] + water_depth[x, y], elev[x, y - 1] + water_depth[x, y - 1]) -
                                            Math.Max(elev[x, y], elev[x, y - 1]);

                            if (hflow > hflow_threshold)
                            {
                                double tempslope = (((elev[x, y - 1] + water_depth[x, y - 1])) -
                                    (elev[x, y] + water_depth[x, y])) / DX;
                                if (y == ymax) tempslope = edgeslope;
                                if (y <= 2 ) tempslope = 0 - edgeslope;

                                //double oldqy = qy[x, y];
                                qy[x, y] = ((qy[x, y] - (gravity * hflow * local_time_factor * tempslope)) /
                                        (1 + gravity * hflow * local_time_factor * (temp_mannings * temp_mannings) * Math.Abs(qy[x, y]) /
                                        Math.Pow(hflow, (10 / 3))));
                                //if (oldqy != 0) qy[x, y] = (oldqy + qy[x, y]) / 2;

                                // need to have these lines to stop too much water moving from one cellt o another - resulting in -ve discharges
                                // whihc causes a large instability to develop - only in steep catchments really
                                if (qy[x, y] > 0 && (qy[x, y] / hflow) / Math.Sqrt(gravity * hflow) > froude_limit ) qy[x, y] = hflow * (Math.Sqrt(gravity * hflow) * froude_limit );
                                if (qy[x, y] < 0 && Math.Abs(qy[x, y] / hflow) / Math.Sqrt(gravity * hflow) > froude_limit ) qy[x, y] = 0 - (hflow * (Math.Sqrt(gravity * hflow) * froude_limit));

                                if (qy[x, y] > 0 && (qy[x, y] * local_time_factor / DX) > (water_depth[x, y] / 4)) qy[x, y] = ((water_depth[x, y] * DX) / 5) / local_time_factor;
                                if (qy[x, y] < 0 && Math.Abs(qy[x, y] * local_time_factor / DX) > (water_depth[x, y - 1] / 4)) qy[x, y] = 0 - ((water_depth[x, y - 1] * DX) / 5) / local_time_factor;


                                if (isSuspended[1])
                                {

                                    if (qy[x, y] > 0) qys[x, y] = qy[x, y] * (Vsusptot[x, y] / water_depth[x, y]);
                                    if (qy[x, y] < 0) qys[x, y] = qy[x, y] * (Vsusptot[x, y - 1] / water_depth[x, y - 1]);

                                    if (qys[x, y] > 0 && qys[x, y] * local_time_factor > (Vsusptot[x, y] * DX) / 4) qys[x, y] = ((Vsusptot[x, y] * DX) / 5) / local_time_factor;
                                    if (qys[x, y] < 0 && Math.Abs(qys[x, y] * local_time_factor) > (Vsusptot[x, y - 1] * DX) / 4) qys[x, y] = 0 - ((Vsusptot[x, y - 1] * DX) / 5) / local_time_factor;

                                }

                                // calc velocity now
                                if (qy[x, y] > 0) vel_dir[x, y, 1] = qy[x, y] / hflow;
                                if (qy[x, y] < 0) vel_dir[x, y - 1, 5] = (0-qy[x, y]) / hflow;
                            }
                            else
                            {
                                qy[x, y] = 0;
                                qys[x, y] = 0;
                            }
                        }

                    }
                }
            });


        }

        void depth_update()
        {
            double local_time_factor = time_factor;
            if (local_time_factor > (courant_number * (DX / Math.Sqrt(gravity * (maxdepth))))) local_time_factor = courant_number * (DX / Math.Sqrt(gravity * (maxdepth)));
            double [] tempmaxdepth2;
            tempmaxdepth2 = new Double[ymax + 2];

            maxdepth = 0;

            var options = new ParallelOptions { MaxDegreeOfParallelism = Environment.ProcessorCount *  4 };
            Parallel.For(1, ymax+1, options, delegate(int y)
            {
                int inc = 1;
                double tempmaxdepth = 0;
                while (down_scan[y, inc] > 0)
                {
                    int x = down_scan[y, inc];
                    inc++;

                    // update water depths
                    water_depth[x,y] += local_time_factor * (qx[x + 1, y] - qx[x, y] + qy[x, y + 1] - qy[x, y]) / DX;
                    // now update SS concs
                    if (isSuspended[1])
                    {
                        Vsusptot[x, y] += local_time_factor * (qxs[x + 1, y] - qxs[x, y] + qys[x, y + 1] - qys[x, y]) / DX;
                    }

                    if (water_depth[x, y] > 0)
                    {
                        // line to remove any water depth on nodata cells (that shouldnt get there!)
                        if (elev[x, y] == -9999) water_depth[x, y] = 0;
                        // calc max flow depth for time step calc
                        if (water_depth[x, y] > tempmaxdepth) tempmaxdepth = water_depth[x, y];
                    }
                }
                tempmaxdepth2[y] = tempmaxdepth;
            });
            // reduction
            for (int y = 1; y <= ymax; y++) if (tempmaxdepth2[y] > maxdepth) maxdepth = tempmaxdepth2[y]; 
        }


        void scan_area()
        {
            double tempW = 0;// water_depth_erosion_threshold;
            var options = new ParallelOptions { MaxDegreeOfParallelism = Environment.ProcessorCount *  4 };
            Parallel.For(1, ymax+1, options, delegate(int y)
            {
                int inc = 1;

                for (int x = 1; x <= xmax; x++)
                {
                    // zero scan bit..
                    down_scan[y, x] = 0;
                    // and work out scanned area.
                    if (water_depth[x, y] > tempW
                        || water_depth[x - 1, y] > tempW
                        //|| water_depth[x - 1, y - 1] > tempW
                        //|| water_depth[x - 1, y + 1] > tempW
                        //|| water_depth[x + 1, y - 1] > tempW
                        //|| water_depth[x + 1, y + 1] > tempW
                        || water_depth[x, y - 1] > tempW
                        || water_depth[x + 1, y] > tempW
                        || water_depth[x, y + 1] > tempW)
                    {
                        down_scan[y, inc] = x;
                        inc++;
                    }
                }
            });

           

            Parallel.For(1, xmax + 1, options, delegate (int x)
            {

                int inc = 1;
                for (int y = 1; y <= ymax; y++)
                {
                    cross_scan[x, y] = 0;
                    if (water_depth[x, y] > tempW
                        || water_depth[x - 1, y] > tempW
                        || water_depth[x + 1, y] > tempW
                        || water_depth[x, y - 1] > tempW
                        || water_depth[x, y + 1] > tempW)
                    {
                        cross_scan[x, inc] = y;
                        inc++;
                    }
                    //discharge[x,y]=0;			
                }

            });


        }

		private void zero_values()
		{
			int x,y,z,n;

			for(y=0;y<=ymax;y++)
			{
				for(x=0;x<=xmax;x++)
				{
                    Vel[x, y] = 0;
					area[x,y]=0;
					elev[x,y]=0;
					bedrock[x,y]=-9999;
					init_elevs[x,y]=elev[x,y];
					water_depth[x,y]=0;
					index[x,y]=-9999;
                    inputpointsarray[x, y] = false;
                    veg[x, y, 0] = 0;// elevation
                    veg[x, y, 1] = 0; // density
					veg[x,y,2]=0; // jw density
					veg[x,y,3]=0; // height
					
					edge[x,y]=0;
					edge2[x,y]=0;
			
					
                    sand[x, y] = 0;

                    qx[x, y] = 0;
                    qy[x, y] = 0;

                    qxs[x, y] = 0;
                    qys[x, y] = 0;

					
                    for(n=0;n<=8;n++)vel_dir[x,y,n]=0;

                    Vsusptot[x, y] = 0;

                    rfarea[x, y] = 1;

                    if (SpatVarManningsCheckbox.Checked == true) spat_var_mannings[x, y] = mannings;
					
				}
			}

			for(x=1;x<((xmax*ymax)/LIMIT);x++)
			{
				for(y=0;y<=G_MAX;y++)
				{
						grain[x,y]=0;
				}
				for(z=0;z<=9;z++)
				{
					for(y=0;y<=G_MAX-2;y++)
					{

							strata[x,z,y]=0;
					}
				}
                catchment_input_x_coord[x]=0;
                catchment_input_y_coord[x]=0;
			}	

            for (x = 1; x <= rfnum; x++)
            {
                j[x] = 0.000000001;
                jo[x] = 0.000000001;
                j_mean[x] = 0;
                old_j_mean[x] = 0;
                new_j_mean[x] = 0;
                M[x] = double.Parse(mvaluebox.Text);
            }
		}

        void dune1(double time) // does dune things from top to bottom
        {
            int dune_recirculate = 0;
            int x, y, n, prob, counter1 = 1, ytemp, ytemp2,x2,y2;
            int t, checkup = int.Parse(upstream_check_box.Text);
            int flag = 1;

            double maxslabdepth = double.Parse(slab_depth_box.Text); 
            int dep_probability= int.Parse(depo_prob_box.Text);
            int downstream_offset = int.Parse(offset_box.Text);
            double number_slabs_per_col = double.Parse(init_depth_box.Text);
            double angle = double.Parse(shadow_angle_box.Text);

            double fractiondune = double.Parse(fraction_dune.Text);
            double slabdepth = maxslabdepth;

            double factor = Math.Tan((angle * (3.141592654 / 180))) * (DX/dune_mult);
            Random xr = new Random();

            double [,] oldelev;
            oldelev = new double[xmax + 2, ymax + 2];

            for (x = 1; x <= xmax; x++)
            {
                for (y = 1; y <= ymax; y++)
                {
                    // subtract eleev from sand so splitting into sand and elev for dune part...
                    oldelev[x, y] = elev[x, y];


                    double sandsum = 0;
                    double sand_diff = 0;
                    for (x2 = 0; x2 < dune_mult; x2++)
                    {
                        for (y2 = 0; y2 < dune_mult; y2++)
                        {
                            // this next line is needed to remove all sand from the far LH and RH cells
                            if (x == xmax) sand2[(x * dune_mult) - x2, (y * dune_mult) - y2] = 0;
                            if (x == 1) sand2[(x * dune_mult) - x2, (y * dune_mult) - y2] = 0;

                            if (sand2[(x * dune_mult) - x2, (y * dune_mult) - y2] > 0)
                            {
                                sandsum += sand2[(x * dune_mult) - x2, (y * dune_mult) - y2];
                            }

                        }
                    }

                    // if water then transfer sand back to elev.
                    if (water_depth[x, y] >= water_depth_erosion_threshold) sand[x, y] = 0;


                    // then check and see if sand is less than sand2 - meaning there has been addition of dune sand to flucial sand
                    if (sand[x, y] < (sandsum / (dune_mult * dune_mult)))
                    {
                        sand_diff = (sandsum / (dune_mult * dune_mult)) - sand[x, y];


                        //if waterdepth >0 then sand > elev and grain (at a rate)
                        //then also reduce sand 2 by same amount transferred.



                        double tempsandvol = sand_diff;// amount to be removed from sand2...

                        // now has to reduce sand vol..
                        double sand2tot = 0;
                        int sand2num = 0;
                        for (x2 = 0; x2 < dune_mult; x2++)
                        {
                            for (y2 = 0; y2 < dune_mult; y2++)
                            {
                                if (sand2[(x * dune_mult) - x2, (y * dune_mult) - y2] > 0)
                                {
                                    sand2tot += sand2[(x * dune_mult) - x2, (y * dune_mult) - y2];
                                    sand2num++;
                                }

                            }
                        }
                        for (x2 = 0; x2 < dune_mult; x2++)
                        {
                            for (y2 = 0; y2 < dune_mult; y2++)
                            {
                                if (sand2[(x * dune_mult) - x2, (y * dune_mult) - y2] > 0)
                                {
                                    sand2[(x * dune_mult) - x2, (y * dune_mult) - y2] -= (tempsandvol * dune_mult * dune_mult) *
                                        (sand2[(x * dune_mult) - x2, (y * dune_mult) - y2] / sand2tot);

                                }

                            }
                        }
                    }
                    
                    elev[x, y] -= sand[x, y];

                    // check to add to sand from grain (if no water)
                    // if so then update sand and sand2 (and elev if subtracting from grain and
                    // adding to sand

                    if (water_depth[x, y] < water_depth_erosion_threshold)
                    {
                        if (elev[x, y] - init_elevs[x, y] > 0)
                        {
                            double tempslabdepth = elev[x, y] - init_elevs[x, y];
                            //if (tempslabdepth < slabdepth) tempslabdepth = 0;
                            if (tempslabdepth > slabdepth) tempslabdepth = slabdepth;
                            sand[x, y] += tempslabdepth;
                            elev[x, y] -= tempslabdepth;

                            // now also update sand2 values... averaged across cell.
                            for (x2 = 0; x2 < dune_mult; x2++)
                            {
                                for (y2 = 0; y2 < dune_mult; y2++)
                                {
                                    sand2[(x * dune_mult) - x2, (y * dune_mult) - y2] += tempslabdepth;
                                }
                            }

                        }

                    }

                    //update elev2 from elev
                    for (x2 = 0; x2 < dune_mult; x2++)
                    {
                        for (y2 = 0; y2 < dune_mult; y2++)
                        {
                            elev2[(x * dune_mult) - x2, (y * dune_mult) - y2] = elev[x, y];
                        }
                    }

                }
            }

            /////////////////////////////////////////////////////////////
            // adding sand part
            /////////////////////////////////////////////////////////////
 
            //if (Math.IEEERemainder((int)(cycle / 60000), 2) == 0)
            //if (Math.IEEERemainder((int)(cycle), 2) == 0)

                for (x = 50; x < (xmax) * dune_mult * fractiondune; x++)
                {
                    //if(Math.IEEERemainder(Math.Abs(x/50)-1,3)==0)sand2[x, 1] = initial_sand_depth;
                    sand2[xr.Next(50, (xmax * dune_mult)), 1] += number_slabs_per_col;

                }

            /////////////////////////////////////////////////////////////
            // end of adding sand code ///
            /////////////////////////////////////////////////////////////


            // check everywhere for sand landslides
            for (x = 1; x <= xmax*dune_mult; x++)
            {
                for (y = 1; y <= ymax*dune_mult; y++)
                {
                    slide_4(x, y);
                }
            }


            for (n = 1; n <= fractiondune * (xmax * dune_mult) * (ymax * dune_mult); n++)
            {

                // creating random x and y co-ords to check...
                x = xr.Next(1, (xmax*dune_mult) + 1);
                y = xr.Next(1, (ymax*dune_mult) + 1);
                prob = 100;
                counter1 = 0;
                flag = 1;

                //if (sand2[x, y] >= slabdepth)
                if (sand2[x, y] >= slabdepth && water_depth[Math.Abs(x / dune_mult), Math.Abs(y / dune_mult)] < water_depth_erosion_threshold)
                {
                    // to see if should be entrained or not...
                    for (t = 1; t <= checkup; t++)
                    {
                        ytemp = y - t;
                        if(dune_recirculate==1 && ytemp < 1) ytemp = (ymax * dune_mult) + ytemp;
                        if (dune_recirculate == 0 && ytemp < 1) ytemp = 1;
                        if (((sand2[x, ytemp] + elev2[x, ytemp]) - (sand2[x, y] + elev2[x, y])) > (factor * (t)))
                        {
                            flag = 0;
                            t = checkup;
                        }

                    }


                    // now having decided it can be eroded, now see if it can be moved
                    if (flag == 1)
                    {
                        // while random number greater than prob then move on one if poss..
                        while (prob > dep_probability)
                        {
                            counter1++; // shift down one cell...
                            prob = xr.Next(1, 100); // new random number

                            //// try moving left and right??
                            //if (xr.Next(1, 100) > 70) x++;
                            //if (xr.Next(1, 100) > 70) x--;
                            //if (x < 1) x = (xmax * dune_mult);
                            //if (x > (xmax * dune_mult)) x = 1;
                            

                            // now adding in the downsrtream offset if needed done by ensuring prob is 100
                            if (counter1 < downstream_offset) prob = 100; // break; 

                            // change comments on below lines if you want water to stop sand movement or not
                            //if (water_depth[Math.Abs(x/dune_mult),Math.Abs((y)/dune_mult)] >= water_depth_erosion_threshold) prob = 0;
                            if (water_depth[Math.Abs(x / dune_mult), Math.Abs((y + Math.Abs(counter1 / dune_mult)) / dune_mult)] >= water_depth_erosion_threshold) prob = 0;

                            // seeing if in shadow or not...
                            for (t = 1; t < checkup; t++)
                            {
                                ytemp = (y + counter1) - t;
                                ytemp2 = (y + counter1);
                                if (dune_recirculate == 0 && ytemp < 1) ytemp = 1;
                                if (dune_recirculate == 1 && ytemp < 1) ytemp = (ymax * dune_mult) + ytemp;
                                if (dune_recirculate == 1)
                                {
                                    if (ytemp > (ymax * dune_mult)) ytemp = 0 + (ytemp - (ymax * dune_mult));
                                    if (ytemp2 > (ymax * dune_mult)) ytemp2 = 0 + (ytemp2 - (ymax * dune_mult));
                                }
                                if (((sand2[x, ytemp] + elev2[x, ytemp]) - (sand2[x, ytemp2] + elev2[x, ytemp2])) > (factor * t))
                                {
                                    t = checkup;
                                    prob = 0;
                                }
                                
                            }
                            //if (elev[x, ytemp - 1] - elev[x, y] > factor) break;
                        }

                        double tempmax = sand2[x, y];
                        if (tempmax < 0) tempmax = 0;



                        // now erode sand if there is enough there in the sand layer
                        if (slabdepth < tempmax) tempmax = slabdepth;
                        if (tempmax < 0) tempmax = 0;
                        sand2[x, y] -= tempmax;
                        // do landslides for just that cell and ones around.
                        slide_4(x, y);

                        ytemp = y + counter1;

                        if (dune_recirculate == 1)
                        {
                            if (ytemp > (ymax * dune_mult))
                            {
                                ytemp = ytemp - (ymax * dune_mult);
                                sand_out += (tempmax * (DX / dune_mult) * (DX / dune_mult));
                            }
                        }

                        // now deposit sand

                        if (ytemp <= ymax * dune_mult)
                        {
                            sand2[x, ytemp] += tempmax;

                            // do landslides for just that cell and ones around.
                            slide_4(x, ytemp);
                        }
                        if(ytemp > (ymax*dune_mult))sand_out+=(tempmax*(DX/dune_mult)*(DX/dune_mult));

                    }
                }

            }




            // now do landslides everywhere 5 times... just to make sure..
            for (t = 1; t <= 5; t++)
            {
                for (x = 1; x <= xmax*dune_mult; x++)
                {
                    for (y = 1; y <= ymax*dune_mult; y++)
                    {
                        if(sand2[x,y]>0)slide_4(x, y);
                    }
                }
            }

            // sand = mean sand2

            for (x = 1; x <= (xmax); x++)
            {
                for (y = 1; y <= (ymax); y++)
                {

                    double sandsum = 0;
                    for (x2 = 0; x2 < dune_mult; x2++)
                    {
                        for (y2 = 0; y2 < dune_mult; y2++)
                        {
                            // this next line is needed to remove all sand from the far LH and RH cells
                            if (x == xmax) sand2[(x * dune_mult) - x2, (y * dune_mult) - y2] = 0;
                            if (x == 1) sand2[(x * dune_mult) - x2, (y * dune_mult) - y2] = 0;

                            if (sand2[(x * dune_mult) - x2, (y * dune_mult) - y2] > 0)
                            {
                                sandsum += sand2[(x * dune_mult) - x2, (y * dune_mult) - y2];
                            }

                        }
                    }


                    sand[x,y] = sandsum/(dune_mult*dune_mult);

                    double newelev = elev[x, y] + sand[x, y];
                    elev[x, y] = oldelev[x, y];
                    elev_diff[x, y] = oldelev[x, y] - newelev;

                }

            }



        }

		void calc_hillshade() // <JOE 20051605- begin>
		{
			//Local variables
			int x, y;
		
			double slopemax;
			double slope;
			int slopetot;
			double local_Illumination;
			
			// Initialize Hillshade Paramaters
			double azimuth = 315 * (3.141592654/180); // Default of 315 degrees converted to radians
			double altitude = 45 * (3.141592654/180); // Default of 45 degrees converted to radians



			for(x=1;x<=xmax;x++)
			{
				for(y=1;y<=ymax;y++)
				{
					if(elev[x,y] != -9999)
					{
						slopemax=0.0;
						slope=0.0;
						slopetot=0;

						// Do slope analysis and Aspect Calculation first
						if(elev[x,y] > elev[x,y-1] && elev[x,y-1] != -9999) // North 0
						{
							slope = Math.Pow((elev[x,y]-elev[x,y-1])/ root,1);
							if(slope > slopemax)
							{
								slopemax=slope;
								slopetot++;
								aspect[x,y] = 0 * (3.141592654/180);
							}	
							
						}
						if(elev[x,y] > elev[x+1,y-1] && elev[x+1,y-1] != -9999) // Northeast 45
						{
							slope = Math.Pow((elev[x,y]-elev[x+1,y-1])/ DX,1);
							if(slope > slopemax)
							{
								slopemax=slope;
								slopetot++;
								aspect[x,y] = 45 * (3.141592654/180);
							}
						}
						if(elev[x,y] > elev[x+1,y] && elev[x+1,y] != -9999) // East 90
						{
							slope = Math.Pow((elev[x,y]-elev[x+1,y])/ root,1);
							if(slope > slopemax)
							{
								slopemax=slope;
								slopetot++;
								aspect[x,y] = 90 * (3.141592654/180);
							}
						}
						if(elev[x,y] > elev[x+1,y+1] && elev[x+1,y+1] != -9999) // SouthEast 135
						{
							slope = Math.Pow((elev[x,y]-elev[x+1,y+1])/ root,1);
							if(slope > slopemax)
							{
								slopemax=slope;
								slopetot++;
								aspect[x,y] = 135 * (3.141592654/180);
							}
							
						}
						if(elev[x,y] > elev[x,y+1] && elev[x,y+1] != -9999) // South 180
						{
							slope = Math.Pow((elev[x,y]-elev[x,y+1])/ DX,1);
							if(slope > slopemax)
							{
								slopemax=slope;
								slopetot++;
								aspect[x,y] = 180 * (3.141592654/180);
							}
						}
						if(elev[x,y] > elev[x-1,y+1] && elev[x-1,y+1] != -9999) // SouthWest 225
						{
							slope = Math.Pow((elev[x,y]-elev[x-1,y+1])/ root,1);
							if(slope > slopemax)
							{
								slopemax=slope;
								slopetot++;
								aspect[x,y] = 225 * (3.141592654/180);
							}
						}
						if(elev[x,y] > elev[x-1,y] && elev[x-1,y] != -9999) // West 270
						{
							slope = Math.Pow((elev[x,y]-elev[x-1,y])/ root,1);
							if(slope > slopemax)
							{
								slopemax=slope;
								slopetot++;
								aspect[x,y] = 270;
							}
						}
						if(elev[x,y] > elev[x-1,y-1] && elev[x-1,y-1] != -9999) // Northwest 315
						{
							slope = Math.Pow((elev[x,y]-elev[x-1,y-1])/ DX,1);
							if(slope > slopemax)
							{
								slopemax=slope;
								slopetot++;
								aspect[x,y] = 315 * (3.141592654/180);
							}
						}
						
						if (slope > 0) slopeAnalysis[x,y] = slopemax;// Tom's: (slope/slopetot); ?

						// Convert slope to radians
						slopeAnalysis[x,y] = System.Math.Atan(slopeAnalysis[x,y]);


						// Do Hillshade Calculation
						local_Illumination = 255 * ((System.Math.Cos(azimuth)
												     * System.Math.Sin(slopeAnalysis[x,y])
												     * System.Math.Cos(aspect[x,y] - azimuth)) 
                            					   + (System.Math.Sin(altitude)
													 * System.Math.Cos(slopeAnalysis[x,y])));
						
						hillshade[x,y] = System.Math.Abs(local_Illumination);
					}
				}
			}

		}		// End calc_hillshade() <JOE 20051605- end>

		void Color_HSVtoRGB()	// <JOE 20051605>
		{
			// Convert HSV to RGB.
			// Made this a seperate function as it is called multiple times in drawwater().

			if (sat == 0) 
			{
				// If sat is 0, all colors are the same.
				// This is some flavor of gray.
				red = val;
				green = val;
				blue = val;
			} 
			else 
			{
				double pFactor;
				double qFactor;
				double tFactor;

				double fractionalSector;
				int sectorNumber;
				double sectorPos;

				// The color wheel consists of six 60 degree sectors.
				// Figure out which sector you are in.
				sectorPos = hue / 60;
				sectorNumber = (int)(Math.Floor(sectorPos));

				// get the fractional part of the sector.
				// That is, how many degrees into the sector are you?
				fractionalSector = sectorPos - sectorNumber;

				// Calculate values for the three axes
				// of the color. 
				pFactor = val * (1 - sat);
				qFactor = val * (1 - (sat * fractionalSector));
				tFactor = val * (1 - (sat * (1 - fractionalSector)));

				// Assign the fractional colors to r, g, and b based on the sector the angle is in.
				switch (sectorNumber) 
				{
					case 0:
						red = val;
						green = tFactor;
						blue = pFactor;
						break;
					case 1:
						red = qFactor;
						green = val;
						blue = pFactor;
						break;
					case 2:
						red = pFactor;
						green = val;
						blue = tFactor;
						break;
					case 3:
						red = pFactor;
						green = qFactor;
						blue = val;
						break;
					case 4:
						red = tFactor;
						green = pFactor;
						blue = val;
						break;
					case 5:
						red = val;
						green = pFactor;
						blue = qFactor;
						break;
				}
			}
		}

		void drawwater(System.Drawing.Graphics graphics)// <JMW 20041018>
		{
            Graphics objGraphics;
            objGraphics = Graphics.FromImage(m_objDrawingSurface);
            objGraphics.Clear(SystemColors.Control);

			int x, y, z,  tot;
			int redcol = 0, greencol = 0, bluecol = 0, alphacol = 255;
			int t=0;
			double tot_max,  tomsedi = 0;


            // load background image....
            //try
            //{
            //    Image tom1 = Image.FromFile(@"img.png");
            //    objGraphics.DrawImage(tom1, 0, 0, xmax * graphics_scale, ymax * graphics_scale);
            //}
            //catch
            //{
            //}
			
			
			// Set Graphics Display Size
			if (xmax <= 0) xmax = 1;

            //set scaling of graphics - so X bmp pixels to every model pixel.
            t = graphics_scale;

			// These loop through the entire grid
			// DEM <JOE 20050905>
			if (menuItem30.Checked == true)	
			{
				double zDEM;
				double zCalc, zMin = 100000.0, zMax = -9990.0, zRange, hsMin = 0, hsMax = 255, hsRange, hs;
				double valMin = 0.0;
				double valMax = 1.0;



				calc_hillshade();		// Call up routine 
				
				// First, find max, min and range of DEM and Hillshade
				for(x=1;x<=xmax;x++)
				{
					for(y=1;y<=ymax;y++)
					{
						zCalc = elev[x,y];
						if(zCalc != -9999)
						{
							if (zCalc < zMin) zMin = zCalc;
							if (zCalc > zMax) zMax = zCalc;
							hs = hillshade[x,y];
							if (hs < hsMin) hsMin = hs;
							if (hs > hsMax) hsMax = hs;
						}
					}
				}
				zRange = zMax - zMin;
				hsRange = hsMax - hsMin;
				
				for(x=1;x<=xmax;x++)
				{
					for(y=1;y<=ymax;y++)
					{
						if (elev[x,y] > -9999.0)
						{
							// HILLSHADE: Draw first underneath
							
							// set gray scale intensity
							hue = 360.0;	// hue doesn't matter for gray shade
							sat = 0.0;		// ensures gray shade
							valMin = 0.0;
							valMax = 1.0;
							val = ((hillshade[x,y]/255) * (valMax - valMin)) + valMin; // uses maximum contrast
//							val = (((hillshade[x,y] - hsMin)/(zRange)) * (valMax - valMin)) + valMin; // lower contrast

							Color_HSVtoRGB();	// Call up color conversion
							redcol = System.Convert.ToInt32(red*255);
							greencol = System.Convert.ToInt32(green*255);
							bluecol = System.Convert.ToInt32(blue*255);
							alphacol = 255;
							
							SolidBrush brush2 = new SolidBrush(Color.FromArgb(alphacol,redcol,greencol,bluecol));
							objGraphics.FillRectangle(brush2,(x-1)*t,(y-1)*t,t,t);

							
							// DEM	
							zDEM = (elev[x,y]);
							// Sets hue based on desired color range (in decimal degrees; max 360)
							double hueMin = 30.0;
							double hueMax = 85.0;
//							hue = (((zDEM - zMin)/(zRange)) * (hueMax - hueMin)) + hueMin; // Forward
							hue = hueMax -(((zDEM - zMin)/(zRange)) * (hueMax - hueMin)); // Reverse
				
							// Set saturation based on desired range
							double satMin = 0.50;
							double satMax = 0.95;
							sat = (((zDEM - zMin)/(zRange)) * (satMax - satMin)) + satMin;
//							sat = 0; // Use for grey-scale DEM only!
							 
							// Set value based on desired range
							valMin = 0.40;
							valMax = 0.80;
							val = (((zDEM - zMin)/(zRange)) * (valMax - valMin)) + valMin;
//							val = valMax - (((zDEM - zMin)/(zRange)) * (valMax - valMin));
						 			
							Color_HSVtoRGB();	// Call up color conversion
							redcol = System.Convert.ToInt32(red*255);
							greencol = System.Convert.ToInt32(green*255);
							bluecol = System.Convert.ToInt32(blue*255);
                            if (redcol < 0) redcol = 0; if (greencol < 0) greencol = 0; if (bluecol < 0) bluecol = 0;
							alphacol = 125;
							
							SolidBrush brush = new SolidBrush(Color.FromArgb(alphacol,redcol,greencol,bluecol));
                            objGraphics.FillRectangle(brush, (x-1) * t, (y-1) * t, t, t);

						}	// Close of Entire Grid Mask
					}		// Close of Column Loop 
				}			// Close of Row Loop

			}				// Close of DEM check box (menuitem34) 
			
            
	// Find Ranges for those in Active Area
			// Water Depth Range:
			double wdCalc, wdMin = 100000.0, wdMax =-10.0, wdRange;
							
			// Find Water Depth Ranges
			for(x=1;x<xmax;x++)
			{
				for(y=1;y<=ymax;y++)
				{
					wdCalc = water_depth[x,y];
					if(wdCalc>0)
					{
						if(wdCalc<wdMin)wdMin=wdCalc;
						if(wdCalc>wdMax)wdMax=wdCalc;
					}
				}
			}
			wdRange = wdMax - wdMin;



			// All these loop through just the 'Active Area'
			for(x=1;x<=xmax;x++)
			{
				for(y=1;y<=ymax;y++)
				{
					if (1>0) // Index masks out so only 'active cells' shown	was if(index[x,y]>-9999)		
					{
						// Water Depth
						if(menuItem3.Checked==true&&water_depth[x,y]>water_depth_erosion_threshold)//MIN_Q)//||discharge[x,y]>0)
						{

							z=0;
                            if (comboBox1.Text == "water depth")
                            {
                                z = (int)(water_depth[x, y] * (256 / wdRange));
                            }
                            else
                            {
                                z = (int)(water_depth[x, y] * 128);
                            }
                         
                            if(z<0)z=0;
							if(z>254)z=254;
							
							greencol=255-z;
							redcol=z;
							bluecol = 255;
                            if (redcol < 0) redcol = 0; if (greencol < 0) greencol = 0; if (bluecol < 0) bluecol = 0;
                            if (redcol > 255) redcol = 255; if (greencol > 255) greencol = 255; if (bluecol > 255) bluecol = 255;

							alphacol = 255;

							SolidBrush brush = new SolidBrush(Color.FromArgb(alphacol,redcol,greencol,bluecol));
                            objGraphics.FillRectangle(brush, (x-1) * t, (y-1) * t, t, t);

						}
					
						// DoD Erosion/ Depostion
						if(menuItem4.Checked==true)
						{
							tomsedi+=(init_elevs[x,y]-elev[x,y]);
							tomsedi-=(Vsusptot[x,y]);
							if(init_elevs[x,y]-elev[x,y]>=0.001) //eroding
							{
                                if (comboBox1.Text == "erosion/dep")
                                {
                                    z = (int)((init_elevs[x, y] - elev[x, y]) * 256 * contrastMultiplier);
                                }
                                else
                                {
                                    z = (int)((init_elevs[x, y] - elev[x, y]) * 64);
                                }
								if(z<0)z=0;
								if(z>254)z=254;
								greencol=255-z;
								redcol=z;
                                if (redcol < 0) redcol = 0; if (greencol < 0) greencol = 0; if (bluecol < 0) bluecol = 0;
                                if (redcol > 255) redcol = 255; if (greencol > 255) greencol = 255; if (bluecol > 255) bluecol = 255;
								// red green blue

								SolidBrush brush = new SolidBrush(Color.FromArgb(255,greencol,greencol));
                                objGraphics.FillRectangle(brush, (x-1) * t, (y-1) * t, t, t);
							}

							if(init_elevs[x,y]-elev[x,y]<=-0.001) //depositing
							{
								z=(int)((elev[x,y]-init_elevs[x,y])*64);
								if(z<0)z=0;
								if(z>254)z=254;
								greencol=z;
								redcol=255-z;
                                if (redcol < 0) redcol = 0; if (greencol < 0) greencol = 0; if (bluecol < 0) bluecol = 0;
                                if (redcol > 255) redcol = 255; if (greencol > 255) greencol = 255; if (bluecol > 255) bluecol = 255;

								SolidBrush brush = new SolidBrush(Color.FromArgb(redcol,255,redcol));
                                objGraphics.FillRectangle(brush, (x-1) * t, (y-1) * t, t, t);
							}

							//						if(init_elevs[x,y]-elev[x,y]>0.0001&&init_elevs[x,y]-elev[x,y]<0.1)graphics.DrawRectangle(Pens.Yellow,x*t,y*t,t,t);
							//						if(init_elevs[x,y]-elev[x,y]>0.1&&init_elevs[x,y]-elev[x,y]<0.2)graphics.DrawRectangle(Pens.Orange,x*t,y*t,t,t);
							//						if(init_elevs[x,y]-elev[x,y]>0.2&&init_elevs[x,y]-elev[x,y]<10)graphics.DrawRectangle(Pens.Red,x*t,y*t,t,t);
							//						if(init_elevs[x,y]-elev[x,y]<-0.0001&&init_elevs[x,y]-elev[x,y]>-0.1)graphics.DrawRectangle(Pens.LightGreen,x*t,y*t,t,t);
							//						if(init_elevs[x,y]-elev[x,y]<-0.1&&init_elevs[x,y]-elev[x,y]>-0.2)graphics.DrawRectangle(Pens.Green,x*t,y*t,t,t);
							//						if(init_elevs[x,y]-elev[x,y]<-0.2&&init_elevs[x,y]-elev[x,y]>-10)graphics.DrawRectangle(Pens.DarkGreen,x*t,y*t,t,t);

						
						}
					
						if(menuItem5.Checked==true)
						{
							//if(water_depth[x,y]>water_depth_erosion_threshold)//if(index[x,y]!=-9999)
							{
							    z=(int)(veg[x,y,1]*256);

                                //z = rfarea[x, y] * 120;
                                //z = (int)(rfarea[x, y] * 12.6);

                                //z = (int)(Vsusptot[x, y] * 20050);
                                //z = (int)(discharge[x,y] * 50.6);
                                //z = (int)(edge2[x,y] * 1000);
                                //z = (int)(Math.Abs(mean_bed_slope (x, y)) * 50 * 256);

                                if (z < 0) z = 0;
                                if (z > 254) z = 254;
                                greencol = z;
                                redcol = 255 - z;
                                if (redcol < 0) redcol = 0; if (greencol < 0) greencol = 0; if (bluecol < 0) bluecol = 0;
                                if (redcol > 255) redcol = 255; if (greencol > 255) greencol = 255; if (bluecol > 255) bluecol = 255;

                                SolidBrush brush = new SolidBrush(Color.FromArgb(redcol, 255, redcol));
                                objGraphics.FillRectangle(brush, (x-1) * t, (y-1) * t, t, t);

							}
						}
					
						
						// now to draw second teir


					if(menuItem7.Checked==true&&edge[x,y]>-9999)
					{

						if(edge[x,y]>=0) //Outside
						{
							z=0;
							z=(int)((edge[x,y])*20000);
							
							if(z<0)z=0;
//							z=(int)((2.131*Math.Pow(edge[x,y],-1.0794))*DX);
//							z=254-z;
							if(z>254)z=254;
							if(z<0)z=0;
							greencol=255-z;
							redcol=z;
                            if (redcol < 0) redcol = 0; if (greencol < 0) greencol = 0; if (bluecol < 0) bluecol = 0;
                            if (redcol > 255) redcol = 255; if (greencol > 255) greencol = 255; if (bluecol > 255) bluecol = 255;
							// red green blue

							SolidBrush brush = new SolidBrush(Color.FromArgb(255,greencol,greencol));
                            objGraphics.FillRectangle(brush, (x-1) * t, (y-1) * t, t, t);
						}

						if(edge[x,y]<0) //inside
						{
							z=(int)((0-edge[x,y])*50);
							if(z<0)z=0;
							if(z>254)z=254;
//							z=(int)((2.131*Math.Pow(edge[x,y],-1.0794))*DX);
//							z=254+z;
//							if(z>254)z=254;
//							if(z<0)z=0;
							greencol=z;
							redcol=255-z;
                            if (redcol < 0) redcol = 0; if (greencol < 0) greencol = 0; if (bluecol < 0) bluecol = 0;
                            if (redcol > 255) redcol = 255; if (greencol > 255) greencol = 255; if (bluecol > 255) bluecol = 255;

							SolidBrush brush = new SolidBrush(Color.FromArgb(redcol,255,redcol));
                            objGraphics.FillRectangle(brush, (x-1) * t, (y-1) * t, t, t);
						}

				
					}

						if(menuItem8.Checked==true)


						{
						
							if(water_depth[x,y]>water_depth_erosion_threshold)
							{
                                if (comboBox1.Text == "Bed sheer stress")
                                {
                                    z = (int)(contrastMultiplier * 5 * Tau[x,y]);
                                }
                                else
                                {
                                    z = (int)(5 * Tau[x,y]);
                                    //z = (int)(Tau[x, y] * (4));
                                }

								if(z<0)z=0;
								if(z>254)z=254;
								//if(z>100)z=254;
								greencol=255-z;
								redcol=z;
                                if (redcol < 0) redcol = 0; if (greencol < 0) greencol = 0; if (bluecol < 0) bluecol = 0;
                                if (redcol > 255) redcol = 255; if (greencol > 255) greencol = 255; if (bluecol > 255) bluecol = 255;

								SolidBrush brush = new SolidBrush(Color.FromArgb(redcol,greencol,255));
                                objGraphics.FillRectangle(brush, (x-1) * t, (y-1) * t, t, t);

							}
						
						}
					
						// D50
						if(menuItem9.Checked==true)
						{
							if(index[x,y]!=-9999)
							{
                                if (comboBox1.Text == "grainsize")
                                {
                                    tot_max = d50(index[x, y]) * 2000*contrastMultiplier;
                                }
                                else
                                {
                                    tot_max = d50(index[x, y]) * 2000;
                                }
								//Console.WriteLine((Convert.ToString(tot_max)));
						
								tot=(int)tot_max;
								if(tot>255)tot=255;
								if(tot<0)tot=0;
                                if (redcol < 0) redcol = 0; if (greencol < 0) greencol = 0; if (bluecol < 0) bluecol = 0;
                                if (redcol > 255) redcol = 255; if (greencol > 255) greencol = 255; if (bluecol > 255) bluecol = 255;

								SolidBrush brush = new SolidBrush(Color.FromArgb(255-(tot*1),255-(tot*1),255-(tot*1)));
                                objGraphics.FillRectangle(brush, (x-1) * t, (y-1) * t, t, t);

							}
						}
					
						
					
						// area drained
						if(menuItem26.Checked==true)
						{
							if(area[x,y]!=-9999)
							{
									z=(int)area[x,y];
								
								
									if(z<0)z=0;
									if(z>254)z=254;

									greencol=255-z;
									redcol=z;
                                    if (redcol < 0) redcol = 0; if (greencol < 0) greencol = 0; if (bluecol < 0) bluecol = 0;
                                    if (redcol > 255) redcol = 255; if (greencol > 255) greencol = 255; if (bluecol > 255) bluecol = 255;

									SolidBrush brush = new SolidBrush(Color.FromArgb(redcol,greencol,255));
                                    objGraphics.FillRectangle(brush, (x-1) * t, (y-1) * t, t, t);

							}
						}
					
						// Suspended Sediment Concentration
						if(menuItem27.Checked==true)
						{
							if(index[x,y]!=-9999)
							{
								if (Vsusptot[x,y] > 0.0)
								{
                                    if (comboBox1.Text == "susp conc")
                                    {
                                        z = (int)(Vsusptot[x, y] * 25600*contrastMultiplier);
                                    }
                                    else
                                    {
                                        z = (int)(Vsusptot[x, y] * 25600);
                                    }
									if(z<0)z=0;
									if(z>254)z=254;

									greencol=255-z;
									redcol=z;
                                    if (redcol < 0) redcol = 0; if (greencol < 0) greencol = 0; if (bluecol < 0) bluecol = 0;
                                    if (redcol > 255) redcol = 255; if (greencol > 255) greencol = 255; if (bluecol > 255) bluecol = 255;

									SolidBrush brush = new SolidBrush(Color.FromArgb(redcol,greencol,255));
                                    objGraphics.FillRectangle(brush, (x-1) * t, (y-1) * t, t, t);
								}
							}
						}

						// Soil depth
						if(menuItem28.Checked==true)
						{
							if(elev[x,y]>-9999)
							{

                                    if (comboBox1.Text == "soildepth")
                                    {
                                        z = (int)((elev[x, y]-bedrock[x,y]) * 2500 * contrastMultiplier);
                                    }
                                    else
                                    {
                                        z = (int)((elev[x, y] - bedrock[x, y]) * 2500);
                                    }
									if(z<0)z=0;
									if(z>254)z=254;
									greencol=255-z;
									redcol=z;
                                    if (redcol < 0) redcol = 0; if (greencol < 0) greencol = 0; if (bluecol < 0) bluecol = 0;
                                    if (redcol > 255) redcol = 255; if (greencol > 255) greencol = 255; if (bluecol > 255) bluecol = 255;

									SolidBrush brush = new SolidBrush(Color.FromArgb(redcol,greencol,255));
                                    objGraphics.FillRectangle(brush, (x-1) * t, (y-1) * t, t, t);

							}
						}
	
						// Flow Velocity
						if (menuItem31.Checked == true)	// <JOE 20050605>
						{
							if (index[x,y] != -9999)
							{
								if (water_depth[x,y]>water_depth_erosion_threshold)
								{
									z=0;
                                    if (comboBox1.Text == "flow velocity")
                                    {
                                        z = (int)((Vel[x,y]) * 125 * contrastMultiplier);
                                    }
                                    else
                                    {
                                        z = (int)((Vel[x, y]) * 125);
                                    }
									//if(yyy>0.25)z=254;
									if (z < 0) z = 0;
									if (z > 254) z = 254;
									greencol = 255-z;
									redcol = z;
                                    if (redcol < 0) redcol = 0; if (greencol < 0) greencol = 0; if (bluecol < 0) bluecol = 0;
                                    if (redcol > 255) redcol = 255; if (greencol > 255) greencol = 255; if (bluecol > 255) bluecol = 255;

									SolidBrush brush = new SolidBrush(Color.FromArgb(redcol,greencol,255));
                                    objGraphics.FillRectangle(brush, (x-1) * t, (y-1) * t, t, t);
								}
							}
						}


					}			// Close of nodata check for 'active' grid only
				}				// Close of Collumn Loop
			}					// Close of Row Loop
            
           
			this.QsStatusPanel.Text=string.Format("Qs = {0:F8}",tomsedi*DX*DX);
            
            objGraphics.Dispose();
            zoomPanImageBox1.Image = m_objDrawingSurface;
            
		}	// Close of drawwater() // <JOE 20051605- end>

		double erode(double mult_factor)
		{
            double rho = 1000.0;
            //double gravity = 9.8;
            double tempbmax = 0;

            double[] gtot2;

            gtot2 = new Double[20];
            
            for (int n = 0; n <= G_MAX; n++)
            {
                gtot2[n] = 0;
            }

            time_factor = time_factor * 1.5;
            if (time_factor > max_time_step) time_factor = max_time_step;

            var options = new ParallelOptions { MaxDegreeOfParallelism = Environment.ProcessorCount * 4 };
            Parallel.For(1, ymax, options, delegate (int y)
            {
                int inc = 1;
                while (down_scan[y, inc] > 0)
                {
                    int x = down_scan[y, inc];
                    inc++;

                    // zero vels.
                    Vel[x, y] = 0;
                    Tau[x, y] = 0;
                    erodetot[x, y] = 0;
                    erodetot3[x, y] = 0;
                    temp_elev[x, y] = 0;

                    for (int n = 0; n < G_MAX; n++)
                    {
                        sr[x, y, n] = 0;
                        sl[x, y, n] = 0;
                        su[x, y, n] = 0;
                        sd[x, y, n] = 0;
                    }
                    ss[x, y] = 0;


                    if (water_depth[x, y] > water_depth_erosion_threshold)
                    {
                        double veltot = 0;
                        double vel = 0;
                        double qtot = 0;
                        double tau = 0;
                        double velnum = 0;
                        double slopetot = 0;



                        // add spatial mannings here
                        double temp_mannings = mannings;
                        if (SpatVarManningsCheckbox.Checked == true) temp_mannings = spat_var_mannings[x, y];

                        // check to see if index for that cell...
                        if (index[x, y] == -9999) addGS(x, y);

                        // now tot up velocity directions, velocities and edge directions.
                        for (int p = 1; p <= 8; p += 2)
                        {
                            int x2 = x + deltaX[p];
                            int y2 = y + deltaY[p];
                            if (water_depth[x2, y2] > water_depth_erosion_threshold)
                            {

                                if (vel_dir[x, y, p] > 0)
                                {
                                    vel = vel_dir[x, y, p];
                                    if (vel > max_vel)
                                    {
                                        this.tempStatusPanel.Text = Convert.ToString(x) + " " + Convert.ToString(y) + " " + Convert.ToString(vel);
                                        vel = max_vel; // if vel too high cut it
                                    }

                                    veltot += vel * vel;
                                    velnum++;
                                    qtot += (vel * vel);
                                    //slopetot += ((elev[x, y] - elev[x2, y2]) / DX);
                                    slopetot += ((elev[x, y] - elev[x2, y2]) / DX) * vel;
                                }
                            }
                        }

                        if (qtot > 0)
                        {
                            vel = (Math.Sqrt(qtot));
                            Vel[x, y] = vel;
                            if (vel < 0)
                            {
                                this.tempStatusPanel.Text = Convert.ToString(x) + " " + Convert.ToString(y) + " " + Convert.ToString(vel);
                            }
                            if (vel > max_vel) vel = max_vel; // if vel too high cut it
                            double ci = gravity * (temp_mannings * temp_mannings) * Math.Pow(water_depth[x, y], -0.33);
                            //tauvel = 1000 * ci * vel * vel;
                            if (slopetot > 0) slopetot = 0;
                            //tauvel = 1000 * ci * vel * vel * (1 + (1 * (slopetot))); 
                            tau = 1000 * ci * vel * vel * (1 + (1 * (slopetot / vel)));
                            Tau[x, y] = tau;
                        }
                    }
                }
            });


            int counter2 = 0;
            do
            {
                counter2++;
                tempbmax = 0;
                double[] tempbmax2;
                tempbmax2 = new Double[ymax + 2];

                //var options = new ParallelOptions { MaxDegreeOfParallelism = Environment.ProcessorCount *  4 };
                Parallel.For(1, ymax, options, delegate (int y)
                {
                    int inc = 1;
                    while (down_scan[y, inc] > 0)
                    {
                        int x = down_scan[y, inc];
                        inc++;

                        // now do some erosion
                        if (Tau[x, y] > 0)
                        {
                            double d_50 = 0;
                            double Fs = 0;
                            double Di = 0;
                            double graintot = 0;
                            double tau = Tau[x, y];
                            double temp_tau_ri = 0;
                            double U_star = 0;
                            double U_star_cubed = 0;

                            double[] temp_dist, tempdir;
                            tempdir = new Double[11]; // array that holds velocity directions temp - so they dont have to be calculated again
                            temp_dist = new Double[11]; // array that holds amount to be removed from cell in each grainsize

                            if (wilcock == 1)
                            {
                                d_50 = d50(index[x, y]);
                                if (d_50 < d1) d_50 = d1;
                                Fs = sand_fraction(index[x, y]);
                                for (int n = 1; n <= G_MAX; n++) graintot += (grain[index[x, y], n]);
                                temp_tau_ri = (0.021 + (0.015 * Math.Exp(-20 * Fs))) * (rho * gravity * d_50);
                                U_star = Math.Pow(tau / rho, 0.5);
                                U_star_cubed = U_star * U_star * U_star;
                            }
                            //////
                            double temptot1 = 0;

                            for (int n = 1; n <= G_MAX - 1; n++)
                            {

                                switch (n)
                                {
                                    case 1: Di = d1; break;
                                    case 2: Di = d2; break;
                                    case 3: Di = d3; break;
                                    case 4: Di = d4; break;
                                    case 5: Di = d5; break;
                                    case 6: Di = d6; break;
                                    case 7: Di = d7; break;
                                    case 8: Di = d8; break;
                                    case 9: Di = d9; break;
                                }

                                // Wilcock and Crowe/Curran

                                
                                if (wilcock == 1)
                                {
                                    Double tau_ri = 0, Wi_star;
                                    tau_ri = temp_tau_ri * Math.Pow((Di / d_50), (0.67 / (1 + Math.Exp(1.5 - (Di / d_50)))));
                                    double Fi = grain[index[x, y], n] / graintot;

                                    if ((tau / tau_ri) < 1.35)
                                    {
                                        Wi_star = 0.002 * Math.Pow(tau / tau_ri, 7.5);
                                    }
                                    else
                                    {
                                        Wi_star = 14 * Math.Pow(1 - (0.894 / Math.Pow(tau / tau_ri, 0.5)), 4.5);
                                    }
                                    //maybe should divide by DX as well..
                                    temp_dist[n] = mult_factor * time_factor *
                                        ((Fi * (U_star_cubed)) / ((2.65 - 1) * gravity)) * Wi_star / DX;
                                }
                                // Einstein sed tpt eqtn
                                if (einstein == 1)
                                {
                                    // maybe should divide by DX as well.. 
                                    temp_dist[n] = mult_factor * time_factor * (40 * Math.Pow((1 / (((2650 - 1000) * Di) / (tau / gravity))), 3))
                                        / Math.Sqrt(1000 / ((2650 - 1000) * gravity * (Di * Di * Di))) / DX;
                                }

                                // Meyer-Peter_Muller set tpt eqtn https://en.m.wikipedia.org/wiki/Sediment_transport#Meyer-Peter_M.C3.BCller_and_derivatives
                                // here using Wiberg and Dungan 89 modification https://dx.doi.org/10.1061%2F%28ASCE%290733-9429%281989%29115%3A1%28101%29
                                if (meyer == 1)
                                {
                                    double TauStar = tau / ((2650 - 1000) * gravity * Di);
                                    if (TauStar > 0.047)
                                    {
                                        temp_dist[n] = mult_factor * time_factor * 9.64 * Math.Pow(TauStar, 0.166) * Math.Pow((TauStar - 0.047), 1.5);
                                    }

                                }



                                //if (temp_dist[n] < 0.0000000000001) temp_dist[n] = 0;

                                // first check to see that theres not too little sediment in a cell to be entrained
                                if (temp_dist[n] > grain[index[x, y], n]) temp_dist[n] = grain[index[x, y], n];
                                // then check to see if this would make SS levels too high.. and if so reduce
                                if (isSuspended[n] && n == 1)
                                {
                                    if ((temp_dist[n] + Vsusptot[x, y]) / water_depth[x, y] > Csuspmax)
                                    {
                                        //work out max amount of sediment that can be there (waterdepth * csuspmax) then subtract whats already there
                                        // (Vsusptot) to leave what can be entrained. Check if < 0 after.
                                        temp_dist[n] = (water_depth[x, y] * Csuspmax) - Vsusptot[x, y];
                                    }
                                }
                                if (temp_dist[n] < 0) temp_dist[n] = 0;

                                // nwo placed here speeding up reduction of erode repeats.
                                temptot1 += temp_dist[n];

                            }


                            //check if this makes it below bedrock
                            if (elev[x, y] - temptot1 <= bedrock[x, y])
                            {
                                // now remove from proportion that can be eroded..
                                // we can do this as we have the prop (in temptot) that is there to be eroded.
                                double elevdiff = elev[x, y] - bedrock[x, y];
                                double temptot3 = temptot1;
                                temptot1 = 0;
                                for (int n = 1; n <= G_MAX - 1; n++)
                                {
                                    if (elev[x, y] <= bedrock[x, y])
                                    {
                                        temp_dist[n] = 0;
                                    }
                                    else
                                    {
                                        temp_dist[n] = elevdiff * (temp_dist[n] / temptot3);
                                        if (temp_dist[n] < 0) temp_dist[n] = 0;
                                    }
                                    temptot1 += temp_dist[n];
                                }

                                // here insert bedrock erosion routine?
                                if (tau > bedrock_erosion_threshold)
                                {
                                    double amount = 0; // amount is amount of erosion into the bedrock.
                                    amount = Math.Pow(bedrock_erosion_rate * tau, 1.5) * time_factor * mult_factor * 0.000000317; // las value to turn it into erosion per year (number of years per second)
                                    bedrock[x, y] -= amount;
                                    // now add amount of bedrock eroded into sediment proportions.
                                    for (int n2 = 1; n2 <= G_MAX - 1; n2++)
                                    {
                                        grain[index[x, y], n2] += amount * dprop[n2];
                                    }
                                }
                            }


                            // veg components
                            // here to erode the veg layer..
                            if (veg[x, y, 1] > 0 && tau > vegTauCrit)
                            {
                                // now to remove from veg layer..
                                veg[x, y, 1] -= mult_factor * time_factor * Math.Pow(tau - vegTauCrit, 0.5) * 0.00001;
                                if (veg[x, y, 1] < 0) veg[x, y, 1] = 0;
                            }

                            //// now to determine if movement should be restricted due to veg... (old method)
                            if (radioButton1.Checked && veg[x, y, 1] > 0.25)
                            {
                                // now checks if this removed from the cell would put it below the veg layer..
                                if (elev[x, y] - temptot1 <= veg[x, y, 0])
                                {
                                    // now remove from proportion that can be eroded..
                                    // we can do this as we have the prop (in temptot) that is there to be eroded.
                                    double elevdiff = 0;
                                    elevdiff = elev[x, y] - veg[x, y, 0];
                                    if (elevdiff < 0) elevdiff = 0;
                                    double temptot3 = temptot1;
                                    temptot1 = 0;
                                    for (int n = 1; n <= G_MAX - 1; n++)
                                    {
                                        temp_dist[n] = elevdiff * (temp_dist[n] / temptot3);
                                        if (elev[x, y] <= veg[x, y, 0]) temp_dist[n] = 0;
                                        temptot1 += temp_dist[n];
                                    }
                                    //temptot1 -= elevdiff;
                                    if (temptot1 < 0) temptot1 = 0;

                                }
                                //tempStatusPanel.Text = Convert.ToString(1);
                            }

                            //// now to determine if movement should be restricted due to veg... (new method)
                            if (radioButton2.Checked && veg[x, y, 1] > 0.1)
                            {
                                temptot1 = 0;
                                for (int n = 1; n <= G_MAX - 1; n++)
                                {
                                    temp_dist[n] *= 1 - (veg[x, y, 1] * (1 - veg_lat_restriction));
                                    temptot1 += temp_dist[n];
                                }
                                //temptot1 -= elevdiff;
                                if (temptot1 < 0) temptot1 = 0;

                                //tempStatusPanel.Text = Convert.ToString(2);
                            }

                            if (temptot1 > tempbmax2[y]) tempbmax2[y] = temptot1;
                            //tempStatusPanel.Text = Convert.ToString(temptot1);

                            // now work out what portion of bedload has to go where...
                            // only allow actual transfer of sediment if there is flow in a direction - i.e. some sedeiment transport

                            // wonder if this part could be separately parallelised?
                            if (temptot1 > 0)
                            {
                                double temptot2 = 0;
                                double veltot = 0;
                                for (int p = 1; p <= 8; p += 2)
                                {
                                    int x2 = x + deltaX[p];
                                    int y2 = y + deltaY[p];
                                    if (water_depth[x2, y2] > water_depth_erosion_threshold)
                                    {
                                        if (edge[x, y] > edge[x2, y2])
                                        {
                                            temptot2 += (edge[x, y] - edge[x2, y2]);
                                        }

                                        if (vel_dir[x, y, p] > 0)
                                        {
                                            // first work out velocities in each direction (for sedi distribution)
                                            double vel = vel_dir[x, y, p];
                                            tempdir[p] = vel * vel;
                                            veltot += tempdir[p];
                                        }
                                    }
                                }


                                for (int p = 1; p <= 8; p += 2)
                                {
                                    int x2 = x + deltaX[p];
                                    int y2 = y + deltaY[p];



                                    if (water_depth[x2, y2] > water_depth_erosion_threshold)
                                    {
                                        if (index[x2, y2] == -9999) addGS(x2, y2);
                                        double factor = 0;

                                        // vel slope
                                        if (vel_dir[x, y, p] > 0)
                                        {
                                            factor += 0.75 * tempdir[p] / veltot;
                                        }
                                        // now for lateral gradient.
                                        if (edge[x, y] > edge[x2, y2])
                                        {
                                            factor += 0.25 * ((edge[x, y] - edge[x2, y2]) / temptot2);
                                        }

                                        // now loop through grainsizes
                                        for (int n = 1; n <= G_MAX - 1; n++)
                                        {
                                            if (temp_dist[n] > 0)
                                            {
                                                if (n == 1 && isSuspended[n])
                                                {
                                                    // put amount entrained by ss in to ss[,]
                                                    ss[x, y] = temp_dist[n];
                                                }
                                                else
                                                {
                                                    switch (p)
                                                    {
                                                        case 1: su[x, y, n] = temp_dist[n] * factor; break;
                                                        case 3: sr[x, y, n] = temp_dist[n] * factor; break;
                                                        case 5: sd[x, y, n] = temp_dist[n] * factor; break;
                                                        case 7: sl[x, y, n] = temp_dist[n] * factor; break;
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                });

                // we have to do a reduction on tempbmax.
                for (int y = 1; y <= ymax; y++) if (tempbmax2[y] > tempbmax) tempbmax = tempbmax2[y];

                if (tempbmax > ERODEFACTOR)
                {
                    time_factor *= (ERODEFACTOR / tempbmax) * 0.5;
                }
            } while(tempbmax > ERODEFACTOR);

            //tempStatusPanel.Text = Convert.ToString(counter2);

            //
            // new temp erode array.


            var options1 = new ParallelOptions { MaxDegreeOfParallelism = Environment.ProcessorCount *  4 };
            Parallel.For(2, ymax, options1, delegate(int y)
            {
                int inc = 1;
                while (down_scan[y, inc] > 0)
                {
                    int x = down_scan[y, inc];
                    inc++;


                    if (water_depth[x, y] > water_depth_erosion_threshold && x < xmax && x > 1)
                    {

                        if (index[x, y] == -9999) addGS(x, y);
                        for (int n = 1; n <= G_MAX-1; n++)
                        {
                            if (n == 1 && isSuspended[n])
                            {
                                // updating entrainment of SS
                                Vsusptot[x, y] += ss[x, y];
                                grain[index[x, y], n] -= ss[x, y];
                                erodetot[x, y] -= ss[x, y];

                                // this next part is unusual. You have to stop susp sed deposition on the input cells, otherwies
                                // it drops sediment out, but cannot entrain as ss levels in input are too high leading to
                                // little mountains of sediment. This means a new array in order to check whether a cell is an 
                                // input point or not..
                                if (!inputpointsarray[x, y])
                                {
                                    // now calc ss to be dropped
                                    double coeff = (fallVelocity[n] * time_factor * mult_factor) / water_depth[x, y];
                                    if (coeff > 1) coeff = 1;
                                    double Vpdrop = coeff * Vsusptot[x, y];
                                    if (Vpdrop > 0.001) Vpdrop = 0.001; //only allow 1mm to be deposited per iteration
                                    grain[index[x, y], n] += Vpdrop;
                                    erodetot[x, y] += Vpdrop;
                                    Vsusptot[x, y] -= Vpdrop;
                                    //if (Vsusptot[x, y] < 0) Vsusptot[x, y] = 0; NOT this line.
                                }
                            }
                            else
                            {
                                //else update grain and elevations for bedload.
                                double val1 = (su[x, y, n] + sr[x, y, n] + sd[x, y, n] + sl[x, y, n]);
                                double val2 = (su[x, y + 1, n] + sd[x, y - 1, n] + sl[x + 1, y, n] + sr[x - 1, y, n]);
                                grain[index[x, y], n] += val2 - val1;
                                erodetot[x, y] += val2 - val1;
                                erodetot3[x, y] += val1;
                            }
                        }

                        temp_elev[x, y] += erodetot[x, y];
                        if (erodetot[x, y] != 0) sort_active(x, y);

                        //
                        // test lateral code...
                        //

                        if (erodetot3[x,y] > 0)
                        {

                            if (elev[x - 1, y] > elev[x, y] && x > 2)
                            {
                                double amt = 0;

                                if (water_depth[x - 1, y] < water_depth_erosion_threshold)
                                    amt = mult_factor * lateral_constant * Tau[x, y] * edge[x - 1, y] * time_factor /DX;
                                else amt = bed_proportion * erodetot3[x, y] * (elev[x - 1, y] - elev[x, y]) / DX * 0.1;

                                if (amt > 0)
                                {
                                    amt *= 1 - (veg[x - 1, y, 1] * (1 - veg_lat_restriction));
                                    if ((elev[x - 1, y] - amt) < bedrock[x - 1, y] || x - 1 == 1) amt = 0;
                                    if (amt > ERODEFACTOR * 0.1) amt = ERODEFACTOR * 0.1;
                                    //if (amt > erodetot2 / 2) amt = erodetot2 / 2;
                                    temp_elev[x, y] += amt;
                                    temp_elev[x - 1, y] -= amt;
                                    slide_GS(x - 1, y, amt, x, y);
                                }
                            }
                            if (elev[x + 1, y] > elev[x, y] && x < xmax-1)
                            {
                                double amt = 0;
                                if (water_depth[x + 1, y] < water_depth_erosion_threshold)
                                    amt = mult_factor * lateral_constant * Tau[x, y] * edge[x + 1, y] * time_factor / DX;
                                else amt = bed_proportion * erodetot3[x, y] * (elev[x + 1, y] - elev[x, y]) / DX * 0.1;

                                if (amt > 0)
                                {
                                    amt *= 1 - (veg[x + 1, y, 1] * (1 - veg_lat_restriction));
                                    if ((elev[x + 1, y] - amt) < bedrock[x + 1, y] || x + 1 == xmax) amt = 0;
                                    if (amt > ERODEFACTOR * 0.1) amt = ERODEFACTOR * 0.1;
                                    //if (amt > erodetot2 /2) amt = erodetot2 /2;
                                    temp_elev[x, y] += amt;
                                    temp_elev[x + 1, y] -= amt;
                                    slide_GS(x + 1, y, amt, x, y);
                                }
                            }

                           
                        }
                    }
                }
            });

            Parallel.For(2, xmax, options1, delegate (int x)
            {
                int inc = 1;
                while (cross_scan[x, inc] > 0)
                {
                    int y = cross_scan[x, inc];
                    inc++;

                    {

                        if (erodetot3[x, y] > 0)
                        {
                            if (elev[x, y - 1] > elev[x, y])
                            {
                                double amt = 0;
                                if (water_depth[x, y - 1] < water_depth_erosion_threshold)
                                    amt = mult_factor * lateral_constant * Tau[x, y] * edge[x, y - 1] * time_factor / DX;
                                else amt = bed_proportion * erodetot3[x, y] * (elev[x, y - 1] - elev[x, y]) / DX * 0.1;

                                if (amt > 0)
                                {
                                    amt *= 1 - (veg[x, y - 1, 1] * (1 - veg_lat_restriction));
                                    if ((elev[x, y - 1] - amt) < bedrock[x, y - 1] || y - 1 == 1) amt = 0;
                                    if (amt > ERODEFACTOR * 0.1) amt = ERODEFACTOR * 0.1;
                                    //if (amt > erodetot2 / 2) amt = erodetot2 / 2;
                                    temp_elev[x, y] += amt;
                                    temp_elev[x, y - 1] -= amt;
                                    slide_GS(x, y - 1, amt, x, y);
                                }
                            }
                            if (elev[x, y + 1] > elev[x, y])
                            {
                                double amt = 0;
                                if (water_depth[x, y + 1] < water_depth_erosion_threshold)
                                    amt = amt = mult_factor * lateral_constant * Tau[x, y] * edge[x, y + 1] * time_factor / DX;
                                else amt = bed_proportion * erodetot3[x, y] * (elev[x, y + 1] - elev[x, y]) / DX * 0.1;

                                if (amt > 0)
                                {
                                    amt *= 1 - (veg[x, y + 1, 1] * (1 - veg_lat_restriction));
                                    if ((elev[x, y + 1] - amt) < bedrock[x, y + 1] || y + 1 == ymax) amt = 0;
                                    if (amt > ERODEFACTOR * 0.1) amt = ERODEFACTOR * 0.1;
                                    //if (amt > erodetot2 / 2) amt = erodetot2 / 2;
                                    temp_elev[x, y] += amt;
                                    temp_elev[x, y + 1] -= amt;
                                    slide_GS(x, y + 1, amt, x, y);
                                }
                            }

                           
                        }
                    }
                }
            });

            Parallel.For(2, ymax, options, delegate (int y)
            {
                int inc = 1;
                while (down_scan[y, inc] > 0)
                {
                    int x = down_scan[y, inc];
                    inc++;
                    
                    if(x>1 && x< xmax) elev[x, y] += temp_elev[x, y];

                }
            });


            // now calculate sediment outputs from all four edges...
            for (int y = 2; y < ymax; y++)
            {
                if (water_depth[xmax, y] > water_depth_erosion_threshold || Vsusptot[xmax, y] > 0)
                {
                    for (int n = 1; n <= G_MAX-1; n++)
                    {
                        if (isSuspended[n])
                        {
                            gtot2[n] += Vsusptot[xmax, y];
                            Vsusptot[xmax, y] = 0;
                        }
                        else
                        {
                            gtot2[n] += sr[xmax - 1, y, n];
                        }
                    }
                }
                if (water_depth[1, y] > water_depth_erosion_threshold || Vsusptot[1, y] > 0)
                {
                    for (int n = 1; n <= G_MAX-1; n++)
                    {
                        if (isSuspended[n])
                        {
                            gtot2[n] += Vsusptot[1, y];
                            Vsusptot[1, y] = 0;
                        }
                        else
                        {
                            gtot2[n] += sl[2, y, n];
                        }
                    }
                }
            }

            for (int x = 2; x < xmax; x++)
            {
                if (water_depth[x, ymax] > water_depth_erosion_threshold || Vsusptot[x, ymax] > 0)
                {
                    for (int n = 1; n <= G_MAX-1; n++)
                    {
                        if (isSuspended[n])
                        {
                            gtot2[n] += Vsusptot[x, ymax];
                            Vsusptot[x, ymax] = 0;
                        }
                        else
                        {
                            gtot2[n] += sd[x, ymax - 1, n];
                        }
                    }
                }
                if (water_depth[x, 1] > water_depth_erosion_threshold || Vsusptot[x, 1] > 0)
                {
                    for (int n = 1; n <= G_MAX-1; n++)
                    {
                        if (isSuspended[n])
                        {
                            gtot2[n] += Vsusptot[x, 1];
                            Vsusptot[x, 1] = 0;
                        }
                        else
                        {
                            gtot2[n] += su[x, 2, n];
                        }
                    }
                }
            }

            /// now update files for outputing sediment and re-circulating...
            /// 

            sediQ = 0;
            for (int n = 1; n <= G_MAX; n++)
            {
                if (temp_grain[n] < 0) temp_grain[n] = 0;
                if (recirculatebox.Checked == true && reach_mode_box.Checked == true)
                    temp_grain[n] += gtot2[n] * recirculate_proportion; // important to divide input by time factor, so it can be reduced if re-circulating too much...
                sediQ += gtot2[n] * DX * DX;
                globalsediq += gtot2[n] * DX * DX;
                sum_grain[n] += gtot2[n] * DX * DX; // Gez
            }

            return tempbmax;

		}

		void slide_3()
		{
			int x,y,inc;
			double wet_factor;
			double factor=Math.Tan((failureangle*(3.141592654/180)))*DX;
			double diff=0;

			for(y=2;y<ymax;y++)
			{
				inc=1;
				while(down_scan[y,inc]>0)
				{
					x=down_scan[y,inc];
					if(x==xmax)x=xmax-1;
					if(x==1)x=2;

					inc++;	
					/** check to see if under water **/
					wet_factor=factor;
					//if(water_depth[x,y]>0.01)wet_factor=factor/2;
					if(elev[x,y]<=(bedrock[x,y]+active))wet_factor=10000;

					/** chexk landslides in channel slowly */

					if(((elev[x,y]-elev[x+1,y+1])/1.41)>wet_factor&&elev[x+1,y+1]> -9999)
					{
						diff=((elev[x,y]-elev[x+1,y+1])/1.41)-wet_factor;
						if((elev[x,y]-diff)<(bedrock[x,y]+active))diff=(elev[x,y]-(bedrock[x,y]+active));
						elev[x,y]-=diff;
						elev[x+1,y+1]+=diff;
						slide_GS(x,y,diff,x+1,y+1);
					}
					if((elev[x,y]-elev[x,y+1])>wet_factor&&elev[x,y+1]> -9999)
					{
						diff=(elev[x,y]-elev[x,y+1])-wet_factor;
						if((elev[x,y]-diff)<(bedrock[x,y]+active))diff=(elev[x,y]-(bedrock[x,y]+active));
						elev[x,y]-=diff;
						elev[x,y+1]+=diff;
						slide_GS(x,y,diff,x,y+1);
					}
					if(((elev[x,y]-elev[x-1,y+1])/1.41)>wet_factor&&elev[x-1,y+1]> -9999)
					{
						diff=((elev[x,y]-elev[x-1,y+1])/1.41)-wet_factor;
						if((elev[x,y]-diff)<(bedrock[x,y]+active))diff=(elev[x,y]-(bedrock[x,y]+active));
						elev[x,y]-=diff;
						elev[x-1,y+1]+=diff;
						slide_GS(x,y,diff,x-1,y+1);
					}
					if((elev[x,y]-elev[x-1,y])>wet_factor&&elev[x-1,y]> -9999)
					{
						diff=(elev[x,y]-elev[x-1,y])-wet_factor;
						if((elev[x,y]-diff)<(bedrock[x,y]+active))diff=(elev[x,y]-(bedrock[x,y]+active));
						elev[x,y]-=diff;
						elev[x-1,y]+=diff;
						slide_GS(x,y,diff,x-1,y);
					}	

					if(((elev[x,y]-elev[x-1,y-1])/1.41)>wet_factor&&elev[x-1,y-1]> -9999)
					{
						diff=((elev[x,y]-elev[x-1,y-1])/1.41)-wet_factor;
						if((elev[x,y]-diff)<(bedrock[x,y]+active))diff=(elev[x,y]-(bedrock[x,y]+active));
						elev[x,y]-=diff;
						elev[x-1,y-1]+=diff;
						slide_GS(x,y,diff,x-1,y-1);
					}
					if((elev[x,y]-elev[x,y-1])>wet_factor&&elev[x,y-1]> -9999)
					{
						diff=(elev[x,y]-elev[x,y-1])-wet_factor;
						if((elev[x,y]-diff)<(bedrock[x,y]+active))diff=(elev[x,y]-(bedrock[x,y]+active));
						elev[x,y]-=diff;
						elev[x,y-1]+=diff;
						slide_GS(x,y,diff,x,y-1);
					}
					if(((elev[x,y]-elev[x+1,y-1])/1.41)>wet_factor&&elev[x+1,y-1]> -9999)
					{
						diff=((elev[x,y]-elev[x+1,y-1])/1.41)-wet_factor;
						if((elev[x,y]-diff)<(bedrock[x,y]+active))diff=(elev[x,y]-(bedrock[x,y]+active));
						elev[x,y]-=diff;
						elev[x+1,y-1]+=diff;
						slide_GS(x,y,diff,x+1,y-1);
					}

                    if ((elev[x, y] - elev[x + 1, y]) > wet_factor && elev[x + 1, y] > -9999)
                    {
                        diff = (elev[x, y] - elev[x + 1, y]) - wet_factor;
                        if ((elev[x, y] - diff) < (bedrock[x, y] + active)) diff = (elev[x, y] - (bedrock[x, y] + active));
                        elev[x, y] -= diff;
                        elev[x + 1, y] += diff;
                        slide_GS(x, y, diff, x + 1, y);
                    }
			
				}
			}

		}

        void slide_5()
        {
            int x, y, inc=0;
            double wet_factor;
            double factor = Math.Tan((failureangle * (3.141592654 / 180))) * DX;
            //if(landslidesBox.Checked == true) factor = DX * ((-265000 * j_mean) + 1.38);
            double diff = 0;
            double total = 0;

            if (DuneBox.Checked == true)
            {
                for (x = 1; x <= xmax; x++)
                {
                    for (y = 1; y <= ymax; y++)
                    {
                        elev[x, y] -= sand[x, y];
                    }
                }
            }



            do
            {
                total = 0;
                inc++;
                for (y = 2; y < ymax; y++)
                {
                    for (x = 2; x < xmax; x++)
                    {

                        wet_factor = factor;
                        //if(water_depth[x,y]>0.01)wet_factor=factor/2;
                        if (elev[x, y] <= (bedrock[x, y] + active)) wet_factor = 10 * DX;

                        /** chexk landslides in channel slowly */

                        if (((elev[x, y] - elev[x + 1, y + 1]) / 1.41) > wet_factor && elev[x + 1, y + 1]> -9999)
                        {
                            diff = ((elev[x, y] - elev[x + 1, y + 1]) / 1.41) - wet_factor;
                            if ((elev[x, y] - diff) < (bedrock[x, y] + active)) diff = (elev[x, y] - (bedrock[x, y] + active));
                            if (diff > ERODEFACTOR) diff = ERODEFACTOR;
                            elev[x, y] -= diff;
                            elev[x + 1, y + 1] += diff;
                            total += diff;
                        }
                        if ((elev[x, y] - elev[x, y + 1]) > wet_factor && elev[x, y + 1]> -9999)
                        {
                            diff = (elev[x, y] - elev[x, y + 1]) - wet_factor;
                            if ((elev[x, y] - diff) < (bedrock[x, y] + active)) diff = (elev[x, y] - (bedrock[x, y] + active));
                            if (diff > ERODEFACTOR) diff = ERODEFACTOR;
                            elev[x, y] -= diff;
                            elev[x, y + 1] += diff;
                            total += diff;
                        }
                        if (((elev[x, y] - elev[x - 1, y + 1]) / 1.41) > wet_factor && elev[x - 1, y + 1]> -9999)
                        {
                            diff = ((elev[x, y] - elev[x - 1, y + 1]) / 1.41) - wet_factor;
                            if ((elev[x, y] - diff) < (bedrock[x, y] + active)) diff = (elev[x, y] - (bedrock[x, y] + active));
                            if (diff > ERODEFACTOR) diff = ERODEFACTOR;
                            elev[x, y] -= diff;
                            elev[x - 1, y + 1] += diff;
                            total += diff;
                        }
                        if ((elev[x, y] - elev[x - 1, y]) > wet_factor && elev[x - 1, y]> -9999)
                        {
                            diff = (elev[x, y] - elev[x - 1, y]) - wet_factor;
                            if ((elev[x, y] - diff) < (bedrock[x, y] + active)) diff = (elev[x, y] - (bedrock[x, y] + active));
                            if (diff > ERODEFACTOR) diff = ERODEFACTOR;
                            elev[x, y] -= diff;
                            elev[x - 1, y] += diff;
                            total += diff;
                        }

                        if (((elev[x, y] - elev[x - 1, y - 1]) / 1.41) > wet_factor && elev[x - 1, y - 1]> -9999)
                        {
                            diff = ((elev[x, y] - elev[x - 1, y - 1]) / 1.41) - wet_factor;
                            if ((elev[x, y] - diff) < (bedrock[x, y] + active)) diff = (elev[x, y] - (bedrock[x, y] + active));
                            if (diff > ERODEFACTOR) diff = ERODEFACTOR;
                            elev[x, y] -= diff;
                            elev[x - 1, y - 1] += diff;
                            total += diff;
                        }
                        if ((elev[x, y] - elev[x, y - 1]) > wet_factor && elev[x, y - 1]> -9999)
                        {
                            diff = (elev[x, y] - elev[x, y - 1]) - wet_factor;
                            if ((elev[x, y] - diff) < (bedrock[x, y] + active)) diff = (elev[x, y] - (bedrock[x, y] + active));
                            if (diff > ERODEFACTOR) diff = ERODEFACTOR;
                            elev[x, y] -= diff;
                            elev[x, y - 1] += diff;
                            total += diff;
                        }
                        if (((elev[x, y] - elev[x + 1, y - 1]) / 1.41) > wet_factor && elev[x + 1, y - 1]> -9999)
                        {
                            diff = ((elev[x, y] - elev[x + 1, y - 1]) / 1.41) - wet_factor;
                            if ((elev[x, y] - diff) < (bedrock[x, y] + active)) diff = (elev[x, y] - (bedrock[x, y] + active));
                            if (diff > ERODEFACTOR) diff = ERODEFACTOR;
                            elev[x, y] -= diff;
                            elev[x + 1, y - 1] += diff;
                            total += diff;
                        }



                        if ((elev[x, y] - elev[x + 1, y]) > wet_factor && elev[x + 1, y]> -9999)
                        {
                            diff = (elev[x, y] - elev[x + 1, y]) - wet_factor;
                            if ((elev[x, y] - diff) < (bedrock[x, y] + active)) diff = (elev[x, y] - (bedrock[x, y] + active));
                            if (diff > ERODEFACTOR) diff = ERODEFACTOR;
                            elev[x, y] -= diff;
                            elev[x + 1, y] += diff;
                            total += diff;
                        }

                    }
                }
            } while (total > 0&&inc<200);

            if (DuneBox.Checked == true)
            {
                for (x = 1; x <= xmax; x++)
                {
                    for (y = 1; y <= ymax; y++)
                    {
                        elev[x, y] += sand[x, y];
                    }
                }
            }

        }

        void slide_4(int x, int y) // landslides from sand dunes...
        {
            double wet_factor;
            double factor = Math.Tan((double.Parse(textBox10.Text) * (3.141592654 / 180))) * (DX/dune_mult);
            double diff = 0;

            wet_factor = factor;



                if ((((elev2[x, y] + sand2[x, y]) - (elev2[x + 1, y + 1] + sand2[x + 1, y + 1])) / 1.41) > wet_factor && (elev2[x + 1, y + 1] + sand2[x + 1, y + 1]) > 0)
                {
                    diff = (((elev2[x, y] + sand2[x, y]) - (elev2[x + 1, y + 1] + sand2[x + 1, y + 1])) / 1.41) - wet_factor;
                    if (diff > sand2[x, y]) diff = sand2[x, y];
                    //if (((elev2[x, y] + sand2[x, y]) - diff) < (bedrock[x, y] + active)) diff = ((elev2[x, y] + sand2[x, y]) - (bedrock[x, y] + active));
                    sand2[x, y] -= diff;
                    sand2[x + 1, y + 1] += diff;
                }
                if (((elev2[x, y] + sand2[x, y]) - (elev2[x, y + 1] + sand2[x, y + 1])) > wet_factor && (elev2[x, y + 1] + sand2[x, y + 1]) > 0)
                {
                    diff = ((elev2[x, y] + sand2[x, y]) - (elev2[x, y + 1] + sand2[x, y + 1])) - wet_factor;
                    if (diff > sand2[x, y]) diff = sand2[x, y];
                    //if (((elev2[x, y] + sand2[x, y]) - diff) < (bedrock[x, y] + active)) diff = ((elev2[x, y] + sand2[x, y]) - (bedrock[x, y] + active));
                    sand2[x, y] -= diff;
                    sand2[x, y + 1] += diff;
                }
                if ((((elev2[x, y] + sand2[x, y]) - (elev2[x - 1, y + 1] + sand2[x - 1, y + 1])) / 1.41) > wet_factor && (elev2[x - 1, y + 1] + sand2[x - 1, y + 1]) > 0)
                {
                    diff = (((elev2[x, y] + sand2[x, y]) - (elev2[x - 1, y + 1] + sand2[x - 1, y + 1])) / 1.41) - wet_factor;
                    if (diff > sand2[x, y]) diff = sand2[x, y];
                    // if (((elev2[x, y] + sand2[x, y]) - diff) < (bedrock[x, y] + active)) diff = ((elev2[x, y] + sand2[x, y]) - (bedrock[x, y] + active));
                    sand2[x, y] -= diff;
                    sand2[x - 1, y + 1] += diff;
                }
                if (((elev2[x, y] + sand2[x, y]) - (elev2[x - 1, y] + sand2[x - 1, y])) > wet_factor && (elev2[x - 1, y] + sand2[x - 1, y]) > 0)
                {
                    diff = ((elev2[x, y] + sand2[x, y]) - (elev2[x - 1, y] + sand2[x - 1, y])) - wet_factor;
                    if (diff > sand2[x, y]) diff = sand2[x, y];
                    //if (((elev2[x, y] + sand2[x, y]) - diff) < (bedrock[x, y] + active)) diff = ((elev2[x, y] + sand2[x, y]) - (bedrock[x, y] + active));
                    sand2[x, y] -= diff;
                    sand2[x - 1, y] += diff;
                }

                if ((((elev2[x, y] + sand2[x, y]) - (elev2[x - 1, y - 1] + sand2[x - 1, y - 1])) / 1.41) > wet_factor && (elev2[x - 1, y - 1] + sand2[x - 1, y - 1]) > 0)
                {
                    diff = (((elev2[x, y] + sand2[x, y]) - (elev2[x - 1, y - 1] + sand2[x - 1, y - 1])) / 1.41) - wet_factor;
                    if (diff > sand2[x, y]) diff = sand2[x, y];
                    //if (((elev2[x, y] + sand2[x, y]) - diff) < (bedrock[x, y] + active)) diff = ((elev2[x, y] + sand2[x, y]) - (bedrock[x, y] + active));
                    sand2[x, y] -= diff;
                    sand2[x - 1, y - 1] += diff;


                }
                if (((elev2[x, y] + sand2[x, y]) - (elev2[x, y - 1] + sand2[x, y - 1])) > wet_factor && (elev2[x, y - 1] + sand2[x, y - 1]) > 0)
                {
                    diff = ((elev2[x, y] + sand2[x, y]) - (elev2[x, y - 1] + sand2[x, y - 1])) - wet_factor;
                    if (diff > sand2[x, y]) diff = sand2[x, y];
                    //if (((elev2[x, y] + sand2[x, y]) - diff) < (bedrock[x, y] + active)) diff = ((elev2[x, y] + sand2[x, y]) - (bedrock[x, y] + active));
                    sand2[x, y] -= diff;
                    sand2[x, y - 1] += diff;


                }
                if ((((elev2[x, y] + sand2[x, y]) - (elev2[x + 1, y - 1] + sand2[x + 1, y - 1])) / 1.41) > wet_factor && (elev2[x + 1, y - 1] + sand2[x + 1, y - 1]) > 0)
                {
                    diff = (((elev2[x, y] + sand2[x, y]) - (elev2[x + 1, y - 1] + sand2[x + 1, y - 1])) / 1.41) - wet_factor;
                    if (diff > sand2[x, y]) diff = sand2[x, y];
                    //if (((elev2[x, y] + sand2[x, y]) - diff) < (bedrock[x, y] + active)) diff = ((elev2[x, y] + sand2[x, y]) - (bedrock[x, y] + active));
                    sand2[x, y] -= diff;
                    sand2[x + 1, y - 1] += diff;

                }


                if (((elev2[x, y] + sand2[x, y]) - (elev2[x + 1, y] + sand2[x + 1, y])) > wet_factor && (elev2[x + 1, y] + sand2[x + 1, y]) > 0)
                {
                    diff = ((elev2[x, y] + sand2[x, y]) - (elev2[x + 1, y] + sand2[x + 1, y])) - wet_factor;
                    if (diff > sand2[x, y]) diff = sand2[x, y];
                    //if (((elev2[x, y] + sand2[x, y]) - diff) < (bedrock[x, y] + active)) diff = ((elev2[x, y] + sand2[x, y]) - (bedrock[x, y] + active));
                    sand2[x, y] -= diff;
                    sand2[x + 1, y] += diff;

                }


        }

		void slide_GS(int x,int y,double amount,int x2,int y2)
		{

            /** Ok, heres how it works, x and y are ones material moved from,
              x2 and y2 are ones material moved to...
              amd amount is the amount shifted. */

            int n;
            double total = 0;

            // do only for cells where both have grainsize..

            if (index[x, y] != -9999 && index[x2, y2] != -9999)
            {
                for (n = 1; n <= (G_MAX - 1); n++)
                {
                    if (grain[index[x, y], n] > 0) total += grain[index[x, y], n];
                }

                if (amount > total)
                {
                    for (n = 1; n <= G_MAX - 1; n++)
                    {
                        grain[index[x2, y2], n] += (amount - total) * dprop[n];
                    }

                    amount = total;
                }

                if (total > 0)
                {
                    for (n = 1; n <= (G_MAX - 1); n++)
                    {
                        double transferamt = amount * (grain[index[x, y], n] / total);
                        grain[index[x2, y2], n] += transferamt;
                        grain[index[x, y], n] -= transferamt;
                        if (grain[index[x, y], n] < 0) grain[index[x, y], n] = 0;
                    }

                }

                /* then to set active layer to correct depth before erosion, */
                sort_active(x, y);
                sort_active(x2, y2);
                return;
            }

            //now do for cells where only recieving cells have grainsize
            // just adds amount to reviving cells of normal..
            if (index[x, y] == -9999 && index[x2, y2] != -9999)
            {
                for (n = 1; n <= G_MAX - 1; n++)
                {
                    grain[index[x2, y2], n] += (amount) * dprop[n];
                }

                /* then to set active layer to correct depth before erosion, */
                sort_active(x2, y2);
                return;
            }

            // now for cells whre dontaing cell has grainsize
            if (index[x, y] != -9999 && index[x2, y2] == -9999)
            {

                addGS(x2, y2); // add grainsize array for recieving cell..

                if (amount > active)
                {

                    for (n = 1; n <= G_MAX - 1; n++)
                    {
                        grain[index[x2, y2], n] += (amount - active) * dprop[n];
                    }

                    amount = active;
                }


                for (n = 1; n <= (G_MAX - 1); n++)
                {
                    if (grain[index[x, y], n] > 0) total += grain[index[x, y], n];
                }

                for (n = 1; n <= (G_MAX - 1); n++)
                {
                    if (total > 0)
                    {
                        grain[index[x2, y2], n] += amount * (grain[index[x, y], n] / total);
                        if (grain[index[x, y], n] > 0.0001) grain[index[x, y], n] -= amount * (grain[index[x, y], n] / total);
                        if (grain[index[x, y], n] < 0) grain[index[x, y], n] = 0;
                    }

                }

                /* then to set active layer to correct depth before erosion, */
                sort_active(x, y);
                sort_active(x2, y2);
                return;
            }
		}

        double mean_ws_elev(int x, int y)
        {
            double elevtot = 0;
            int counter = 0;

            for (int dir = 1; dir <= 8; dir++)
            {
                int x2, y2;
                x2 = x + deltaX[dir];
                y2 = y + deltaY[dir];

                if (water_depth[x2, y2] > water_depth_erosion_threshold)
                {
                    elevtot += water_depth[x2, y2] + elev[x2, y2];
                    counter++;
                }

            }
            if (counter > 0) {
                elevtot /= counter;
                return elevtot;
            }

            else return 0;
        }

        void lateral3()
        {

            double[,] edge_temp, edge_temp2, water_depth2;
            int[,] upscale, upscale_edge;

            edge_temp = new Double[xmax + 1, ymax + 1];
            edge_temp2 = new Double[xmax + 1, ymax + 1];
            water_depth2 = new Double[xmax + 1, ymax + 1];
            upscale = new int[(xmax + 1) * 2, (ymax + 1) * 2];
            upscale_edge = new int[(xmax + 1) * 2, (ymax + 1) * 2];


            // first make water depth2 equal to water depth then remove single wet cells frmo water depth2 that have an undue influence..
            double mft = 0.1;// water_depth_erosion_threshold;//MIN_Q;// vel_dir threshold

            var options = new ParallelOptions { MaxDegreeOfParallelism = Environment.ProcessorCount * 4 };
            Parallel.For(1, ymax, options, delegate (int y)
            {
                int inc = 1;
                while (down_scan[y, inc] > 0)
                {
                    int x = down_scan[y, inc];

                    edge_temp[x, y] = 0;
                    if (x == 1) x++;
                    if (x == xmax) x--;
                    inc++;

                    if (Tau[x, y] > mft)
                    {
                        water_depth2[x, y] = Tau[x, y];
                        int tempcounter = 0;
                        for (int dir = 1; dir <= 8; dir++)
                        {
                            int x2, y2;
                            x2 = x + deltaX[dir];
                            y2 = y + deltaY[dir];
                            if (Tau[x2, y2] < mft) tempcounter++;
                        }
                        if (tempcounter > 6) water_depth2[x, y] = 0;
                    }
                }
            });

            // first make water depth2 equal to water depth then remove single wet cells frmo water depth2 that have an undue influence..
            //double mft = water_depth_erosion_threshold;//MIN_Q;// vel_dir threshold
            //for (int y = 2; y < ymax; y++)
            //{

            //    int inc = 1;
            //    while (down_scan[y, inc] > 0)
            //    {
            //        int x = down_scan[y, inc];

            //        edge_temp[x, y] = 0;
            //        if (x == 1) x++;
            //        if (x == xmax) x--;
            //        inc++;

            //        if (water_depth[x, y] > mft)
            //        {
            //            water_depth2[x, y] = water_depth[x, y];
            //            int tempcounter = 0;
            //            for (int dir = 1; dir <= 8; dir++)
            //            {
            //                int x2, y2;
            //                x2 = x + deltaX[dir];
            //                y2 = y + deltaY[dir];
            //                if (water_depth[x2, y2] < mft) tempcounter++;
            //            }
            //            if (tempcounter > 6) water_depth2[x, y] = 0;
            //        }
            //    }
            //}


            // first determine which cells are at the edge of the channel

            //var options = new ParallelOptions { MaxDegreeOfParallelism = Environment.ProcessorCount * 4 };
            Parallel.For(2, ymax, options, delegate (int y)
            {
                Parallel.For(2, xmax, options, delegate (int x)
                {
                    edge[x, y] = -9999;

                    if (water_depth2[x, y] < mft)
                    {
                        // if water depth < threshold then if its next to a wet cell then its an edge cell
                        if (water_depth2[x, y - 1] > mft ||
                            water_depth2[x - 1, y] > mft ||
                            water_depth2[x + 1, y] > mft ||
                            water_depth2[x, y + 1] > mft)
                        {
                            edge[x, y] = 0;
                        }

                        // unless its a dry cell surrounded by wet...
                        if (water_depth2[x, y - 1] > mft &&
                            water_depth2[x - 1, y] > mft &&
                            water_depth2[x + 1, y] > mft &&
                            water_depth2[x, y + 1] > mft)
                        {
                            edge[x, y] = -9999;
                            edge2[x, y] = -9999;
                        }

                        // then update upscaled grid..
                        upscale[(x * 2), (y * 2)] = 0; // if dry
                        upscale[(x * 2), (y * 2) - 1] = 0;
                        upscale[(x * 2) - 1, (y * 2)] = 0;
                        upscale[(x * 2) - 1, (y * 2) - 1] = 0;
                    }

                    // update upscaled grid with wet cells (if wet)
                    if (water_depth2[x, y] >= mft)
                    {
                        upscale[(x * 2), (y * 2)] = 1; // if wet
                        upscale[(x * 2), (y * 2) - 1] = 1;
                        upscale[(x * 2) - 1, (y * 2)] = 1;
                        upscale[(x * 2) - 1, (y * 2) - 1] = 1;
                    }
                });
            });



            // now determine edge cells on the new grid..

            Parallel.For(2, ymax*2, options, delegate (int y)
            {
                Parallel.For(2, xmax*2, options, delegate (int x)
                {
                    upscale_edge[x, y] = 0;
                    if (upscale[x, y] == 0)
                    {
                        if (upscale[x, y - 1] == 1 ||
                            upscale[x - 1, y] == 1 ||
                            upscale[x + 1, y] == 1 ||
                            upscale[x, y + 1] == 1)
                        {
                            upscale[x, y] = 2;
                        }
                    }
                });

            });



            // now tall up inside and outside on upscaled grid

            Parallel.For(2, ymax * 2, options, delegate (int y)
            {
                Parallel.For(2, xmax * 2, options, delegate (int x)
                {
                    if (upscale[x, y] == 2)
                    {
                        int wetcells = 0;
                        int drycells = 0;
                        int water = 0;
                        int edge_cell_counter = 1;

                        // sum up dry cells and edge cells - 
                        // now manhattan neighbors
                        for (int dir = 1; dir <= 7; dir += 2)
                        {
                            int x2, y2;
                            x2 = x + deltaX[dir];
                            y2 = y + deltaY[dir];

                            if (upscale[x2, y2] == 1) wetcells += 1;
                            if (upscale[x2, y2] == 0) drycells += 1;
                            if (upscale[x2, y2] == 2) edge_cell_counter += 1;
                        }

                        if (edge_cell_counter > 3) drycells += edge_cell_counter - 2;
                        //						
                        water = wetcells - drycells;
                        upscale_edge[x, y] = water;
                    }
                });

            });


            // now update normal edge array..

            Parallel.For(1, ymax+1, options, delegate (int y)
            {
                Parallel.For(1, xmax+1, options, delegate (int x)
                {
                    if (edge[x, y] == 0)
                    {
                        edge[x, y] = (double)(upscale_edge[(x * 2), (y * 2)] +
                            upscale_edge[(x * 2), (y * 2) - 1] +
                            upscale_edge[(x * 2) - 1, (y * 2)] +
                            upscale_edge[(x * 2) - 1, (y * 2) - 1]);
                        if (edge[x, y] > 2) edge[x, y] = 2; // important line to stop too great inside bends...
                        if (edge[x, y] < -2) edge[x, y] = -2;

                    }
                });
            });

            //then apply a smoothing filter over the top of this. here its done X number of times - 

            double smoothing_times = double.Parse(avge_smoothbox.Text);
            double downstream_shift = double.Parse(downstreamshiftbox.Text);

            for (int n = 1; n <= smoothing_times+downstream_shift; n++)
            {
                //var options = new ParallelOptions { MaxDegreeOfParallelism = Environment.ProcessorCount *  4 };
                Parallel.For(2, ymax, options, delegate(int y)
                {
                    int inc = 1;
                    while (down_scan[y, inc] > 0)
                    {
                        int x = down_scan[y, inc];

                        edge_temp[x, y] = 0;
                        if (x == 1) x++;
                        if (x == xmax) x--;
                        if (y == 1) y++;
                        if (y == ymax) y--;
                        inc++;

                        if (edge[x, y] > -9999)
                        {
                            double mean = 0;
                            double num = 0;
                            double water_flag = 0;


                            // add in cell itself..
                            mean += edge[x, y];
                            num++;


                            for (int dir = 1; dir <= 8; dir++)
                            {
                                int x2, y2;
                                x2 = x + deltaX[dir];
                                y2 = y + deltaY[dir];
                                if (water_depth2[x2, y2] > mft) water_flag++;

                                if ( n > smoothing_times && edge[x2, y2] > -9999 && water_depth2[x2, y2] < mft && mean_ws_elev(x2,y2)>mean_ws_elev(x,y))
                                {
                                    //now to mean manhattan neighbours - only if they share a wet diagonal neighbour
                                    if ((Math.Abs(deltaX[dir]) + Math.Abs(deltaY[dir])) != 2)
                                    {
                                        if (deltaX[dir] == 1 && deltaY[dir] == 0 &&
                                            (water_depth2[x + 1, y - 1] > mft ||
                                            water_depth2[x + 1, y + 1] > mft))
                                        {
                                            mean += (edge[x + deltaX[dir], y + deltaY[dir]]);
                                            num++;
                                        }
                                        if (deltaX[dir] == 0 && deltaY[dir] == 1 &&
                                            (water_depth2[x + 1, y + 1] > mft ||
                                            water_depth2[x - 1, y + 1] > mft))
                                        {
                                            mean += (edge[x + deltaX[dir], y + deltaY[dir]]);
                                            num++;
                                        }
                                        if (deltaX[dir] == -1 && deltaY[dir] == 0 &&
                                            (water_depth2[x - 1, y - 1] > mft ||
                                            water_depth2[x - 1, y + 1] > mft))
                                        {
                                            mean += (edge[x + deltaX[dir], y + deltaY[dir]]);
                                            num++;
                                        }
                                        if (deltaX[dir] == 0 && deltaY[dir] == -1 &&
                                            (water_depth2[x - 1, y - 1] > mft ||
                                            water_depth2[x + 1, y - 1] > mft))
                                        {
                                            mean += (edge[x + deltaX[dir], y + deltaY[dir]]);
                                            num++;
                                        }
                                    }
                                    //now non manahttan neighbours, with concected by a dry cell checked..
                                    else
                                    {
                                        if (deltaX[dir] == -1 && deltaY[dir] == -1 &&
                                            (water_depth2[x, y - 1] < mft ||
                                            water_depth2[x - 1, y] < mft))
                                        {
                                            mean += (edge[x + deltaX[dir], y + deltaY[dir]]);
                                            num++;
                                        }
                                        if (deltaX[dir] == 1 && deltaY[dir] == -1 &&
                                            (water_depth2[x, y - 1] < mft ||
                                            water_depth2[x + 1, y] < mft))
                                        {
                                            mean += (edge[x + deltaX[dir], y + deltaY[dir]]);
                                            num++;
                                        }
                                        if (deltaX[dir] == 1 && deltaY[dir] == 1 &&
                                            (water_depth2[x + 1, y] < mft ||
                                            water_depth2[x, y + 1] < mft))
                                        {
                                            mean += (edge[x + deltaX[dir], y + deltaY[dir]]);
                                            num++;
                                        }
                                        if (deltaX[dir] == -1 && deltaY[dir] == 1 &&
                                            (water_depth2[x, y + 1] < mft ||
                                            water_depth2[x - 1, y] < mft))
                                        {
                                            mean += (edge[x + deltaX[dir], y + deltaY[dir]]);
                                            num++;
                                        }
                                    }
                                }

                                else if ( n <= smoothing_times && edge[x2, y2] > -9999 && water_depth2[x2, y2] < mft)
                                {
                                    //now to mean manhattan neighbours - only if they share a wet diagonal neighbour
                                    if ((Math.Abs(deltaX[dir]) + Math.Abs(deltaY[dir])) != 2)
                                    {
                                        if (deltaX[dir] == 1 && deltaY[dir] == 0 &&
                                            (water_depth2[x + 1, y - 1] > mft ||
                                            water_depth2[x + 1, y + 1] > mft))
                                        {
                                            mean += (edge[x + deltaX[dir], y + deltaY[dir]]);
                                            num++;
                                        }
                                        if (deltaX[dir] == 0 && deltaY[dir] == 1 &&
                                            (water_depth2[x + 1, y + 1] > mft ||
                                            water_depth2[x - 1, y + 1] > mft))
                                        {
                                            mean += (edge[x + deltaX[dir], y + deltaY[dir]]);
                                            num++;
                                        }
                                        if (deltaX[dir] == -1 && deltaY[dir] == 0 &&
                                            (water_depth2[x - 1, y - 1] > mft ||
                                            water_depth2[x - 1, y + 1] > mft))
                                        {
                                            mean += (edge[x + deltaX[dir], y + deltaY[dir]]);
                                            num++;
                                        }
                                        if (deltaX[dir] == 0 && deltaY[dir] == -1 &&
                                            (water_depth2[x - 1, y - 1] > mft ||
                                            water_depth2[x + 1, y - 1] > mft))
                                        {
                                            mean += (edge[x + deltaX[dir], y + deltaY[dir]]);
                                            num++;
                                        }
                                    }
                                    //now non manahttan neighbours, with concected by a dry cell checked..
                                    else
                                    {
                                        if (deltaX[dir] == -1 && deltaY[dir] == -1 &&
                                            (water_depth2[x, y - 1] < mft ||
                                            water_depth2[x - 1, y] < mft))
                                        {
                                            mean += (edge[x + deltaX[dir], y + deltaY[dir]]);
                                            num++;
                                        }
                                        if (deltaX[dir] == 1 && deltaY[dir] == -1 &&
                                            (water_depth2[x, y - 1] < mft ||
                                            water_depth2[x + 1, y] < mft))
                                        {
                                            mean += (edge[x + deltaX[dir], y + deltaY[dir]]);
                                            num++;
                                        }
                                        if (deltaX[dir] == 1 && deltaY[dir] == 1 &&
                                            (water_depth2[x + 1, y] < mft ||
                                            water_depth2[x, y + 1] < mft))
                                        {
                                            mean += (edge[x + deltaX[dir], y + deltaY[dir]]);
                                            num++;
                                        }
                                        if (deltaX[dir] == -1 && deltaY[dir] == 1 &&
                                            (water_depth2[x, y + 1] < mft ||
                                            water_depth2[x - 1, y] < mft))
                                        {
                                            mean += (edge[x + deltaX[dir], y + deltaY[dir]]);
                                            num++;
                                        }
                                    }
                                }
                            }
                            if (mean != 0) edge_temp[x, y] = mean / num;

                            // removes too many cells - islands etc..

                            //if(num>5&&edge[x,y]>0)edge_temp[x,y]=0;
                            //if(num+water_flag>7&&edge[x,y]>0)edge_temp[x,y]=0;

                            //remove edge effects
                            if (x < 3 || x > (xmax - 3)) edge_temp[x, y] = 0;
                            if (y < 3 || y > (ymax - 3)) edge_temp[x, y] = 0;

                        }
                    }
                });

                Parallel.For(2, ymax, options, delegate (int y)
                {
                    int inc = 1;
                    while (down_scan[y, inc] > 0)
                    {
                        int x = down_scan[y, inc];
                        //if (x == 1) x++;
                        //if (x == xmax) x--;
                        inc++;
                        if (edge[x, y] > -9999)
                        {
                            edge[x, y] = edge_temp[x, y];
                        }
                    }
                });
            }


            // trial line to remove too high inside bends,,
            Parallel.For(1, ymax + 1, options, delegate (int y)
            {
                Parallel.For(1, xmax + 1, options, delegate (int x)
                {
                    if (edge[x, y] > -9999)
                    {
                        if (edge[x, y] > 0) edge[x, y] = 0;
                        //if (edge[x, y] < -0.25) edge[x, y] = -0.25;
                        edge[x, y] = 0 - edge[x, y];
                        edge[x, y] = 1 / ((2.131 * Math.Pow(edge[x, y], -1.0794)) * DX);
                        //if (edge[x, y] > (1 / (DX * 3))) edge[x, y] = 1 / (DX * 3);
                        //edge[x, y] = 1 / edge[x, y]; 

                    }
                    if (water_depth[x, y] > water_depth_erosion_threshold && edge[x, y] == -9999) edge[x, y] = 0;
                });
            });

            //// now smooth across the channel..
            double tempdiff = 0;
            double counter = 0;
            do
            {
                counter++;
                //var options = new ParallelOptions { MaxDegreeOfParallelism = Environment.ProcessorCount *  4 };
                Parallel.For(2, ymax, options, delegate(int y)
                {
                    int inc = 1;
                    while (down_scan[y, inc] > 0)
                    {
                        int x = down_scan[y, inc];

                        edge_temp[x, y] = 0;
                        if (x == 1) x++;
                        if (x == xmax) x--;
                        inc++;
                        if (water_depth2[x, y] > mft && edge[x, y] == -9999) edge[x, y] = 0;

                        if (edge[x, y] > -9999 && water_depth2[x, y] > mft)
                        {
                            double mean = 0;
                            int num = 0;
                            for (int dir = 1; dir <= 8; dir+=2)
                            {
                                int x2, y2;
                                x2 = x + deltaX[dir];
                                y2 = y + deltaY[dir];

                                if (water_depth2[x2, y2] > mft && edge[x2, y2] == -9999) edge[x2, y2] = 0;
                                if (edge[x2, y2] > -9999)
                                {
                                    mean += (edge[x2, y2]);
                                    num++;
                                }
                            }
                            edge_temp[x, y] = mean / num;
                        }
                    }
                });

                tempdiff = 0;
                //Parallel.For(2, ymax, options, delegate (int y)
                //{

                // reduction needed here:
                for (int y = 2; y < ymax; y++)
                {
                    int inc = 1;
                    while (down_scan[y, inc] > 0)
                    {
                        int x = down_scan[y, inc];
                        if (x == 1) x++;
                        if (x == xmax) x--;
                        inc++;
                        if (edge[x, y] > -9999 && water_depth2[x, y] > mft)
                        {
                            if (Math.Abs(edge[x, y] - edge_temp[x, y]) > tempdiff) tempdiff = Math.Abs(edge[x, y] - edge_temp[x, y]);
                            edge[x, y] = edge_temp[x, y];
                        }
                    }
                }
                //});
            } while (tempdiff > lateral_cross_channel_smoothing); //this makes it loop until the averaging across the stream stabilises
            // so that the difference between the old and new values are < 0.0001
            //tempStatusPanel.Text = Convert.ToString(counter);

        }

		private void Form1_Load(object sender, System.EventArgs e)
		{

            zoomPanImageBox1.Height = this.Height - 225;
            zoomPanImageBox1.Width = this.Width - 20;
            //googleToggle();

            //HttpWebRequest req;
            //HttpWebResponse res;
            //try
            //{
            //    req = (HttpWebRequest) WebRequest.Create("http://www.coulthard.org.uk/");
            //    res = (HttpWebResponse) req.GetResponse();
            //} 
            //catch(Exception ex)
            //{
            //    /// do nothing.
            //}

			//JMW <20040929 -start>
			this.Text = basetext;
			//DoingGraphics = false;
			//JMW <20040929 - end>


            // comment out all of the below to run normally. Leave uncommented in order to run in batch mode.
            ////////////////////////
            //////////////////////////



            ///// first load up xml file from command line:
            ///// 
            //string temp_xml_name = " ";

            //foreach (string arg in Environment.GetCommandLineArgs())
            //{
            //    if (arg != "CAESAR.exe") temp_xml_name = arg;
            //}

            ///// then load up .xml file
            ///// 

            //XmlTextReader xreader;
            //String dum;

            //if (1 > 0)
            //{

            //    xreader = new XmlTextReader(temp_xml_name);

            //    //Read the file
            //    if (xreader != null)
            //    {
            //        xreader.ReadStartElement("Parms");
            //        xreader.ReadStartElement("General-Parms");
            //        try
            //        {
            //            overrideheaderBox.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("headeroverride"));
            //        }
            //        catch
            //        { };
            //        xtextbox.Text = xreader.ReadElementString("x-coordinate");
            //        ytextbox.Text = xreader.ReadElementString("y-coordinate");
            //        initscansbox.Text = xreader.ReadElementString("initscans");
            //        erodefactorbox.Text = xreader.ReadElementString("maxerodelimit");
            //        dxbox.Text = xreader.ReadElementString("cellsize");
            //        limitbox.Text = xreader.ReadElementString("memorylimit");
            //        minqbox.Text = xreader.ReadElementString("minq");
            //        creepratebox.Text = xreader.ReadElementString("creeprate");
            //        lateralratebox.Text = xreader.ReadElementString("lateralerosionrate");
            //        itermaxbox.Text = xreader.ReadElementString("maxiter");
            //        textBox1.Text = xreader.ReadElementString("runstarttime");
            //        cyclemaxbox.Text = xreader.ReadElementString("maxrunduration");
            //        slopebox.Text = xreader.ReadElementString("slopefailurethreshold");
            //        smoothbox.Text = xreader.ReadElementString("wssmoothingradius");
            //        mvaluebox.Text = xreader.ReadElementString("mvalue");

            //        grasstextbox.Text = xreader.ReadElementString("growgrasstime");
            //        textBox2.Text = xreader.ReadElementString("initialq");
            //        try
            //        {
            //            checkBox3.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("wssmoothing"));
            //        }
            //        catch
            //        { };
            //        grassbox.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("grass-sediment"));

            //        try // MJ 24/01/05
            //        {
            //            textBox3.Text = xreader.ReadElementString("flowdistribution");
            //            mintimestepbox.Text = xreader.ReadElementString("mintimestep");
            //        }
            //        catch
            //        { };

            //        try // MJ 15/03/05
            //        {
            //            k_evapBox.Text = xreader.ReadElementString("evaporation");
            //        }
            //        catch
            //        { };

            //        try // MJ 10/05/05
            //        {
            //            vegTauCritBox.Text = xreader.ReadElementString("vegcritshear");
            //        }
            //        catch
            //        { };

            //        try
            //        {
            //            bedslope_box.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("bedslope"));
            //        }
            //        catch
            //        { };
            //        try
            //        {
            //            bool dum_bool = XmlConvert.ToBoolean(xreader.ReadElementString("wsslope"));
            //        }
            //        catch
            //        { };
            //        try
            //        {
            //            veltaubox.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("veltaubox"));
            //        }
            //        catch
            //        { };
            //        try
            //        {
            //            catchment_mode_box.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("catchment_mode"));
            //        }
            //        catch
            //        { };
            //        try
            //        {
            //            reach_mode_box.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("reach_mode"));
            //        }
            //        catch
            //        { };
            //        try
            //        {
            //            latbox1.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("lat1"));
            //        }
            //        catch
            //        { };
            //        try
            //        {
            //            latbox2.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("lat2"));
            //        }
            //        catch
            //        { };
            //        try
            //        {
            //            bool dum_bool = XmlConvert.ToBoolean(xreader.ReadElementString("lat3"));
            //        }
            //        catch
            //        { };
            //        try
            //        {
            //            string dum_string = xreader.ReadElementString("cross_stream_grad");
            //        }
            //        catch
            //        { };
            //        try
            //        {
            //            max_vel_box.Text = xreader.ReadElementString("max_vel");
            //        }
            //        catch { };


            //        xreader.ReadStartElement("SaveOptions");
            //        dum = xreader.ReadElementString("Option");
            //        menuItem12.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("Checked"));
            //        xreader.ReadEndElement();
            //        xreader.ReadStartElement("SaveOptions");
            //        dum = xreader.ReadElementString("Option");
            //        menuItem13.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("Checked"));
            //        xreader.ReadEndElement();
            //        xreader.ReadStartElement("SaveOptions");
            //        dum = xreader.ReadElementString("Option");
            //        menuItem14.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("Checked"));
            //        xreader.ReadEndElement();
            //        xreader.ReadStartElement("SaveOptions");
            //        dum = xreader.ReadElementString("Option");
            //        menuItem15.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("Checked"));
            //        xreader.ReadEndElement();
            //        xreader.ReadStartElement("SaveOptions");
            //        dum = xreader.ReadElementString("Option");
            //        menuItem16.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("Checked"));
            //        xreader.ReadEndElement();
            //        xreader.ReadStartElement("SaveOptions");
            //        dum = xreader.ReadElementString("Option");
            //        menuItem17.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("Checked"));
            //        xreader.ReadEndElement();
            //        xreader.ReadStartElement("SaveOptions");
            //        dum = xreader.ReadElementString("Option");
            //        menuItem18.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("Checked"));
            //        xreader.ReadEndElement();
            //        xreader.ReadStartElement("SaveOptions");
            //        dum = xreader.ReadElementString("Option");
            //        menuItem19.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("Checked"));
            //        xreader.ReadEndElement();
            //        xreader.ReadStartElement("SaveOptions");
            //        dum = xreader.ReadElementString("Option");
            //        menuItem20.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("Checked"));
            //        xreader.ReadEndElement();
            //        xreader.ReadStartElement("SaveOptions");
            //        dum = xreader.ReadElementString("Option");
            //        menuItem21.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("Checked"));
            //        xreader.ReadEndElement();
            //        xreader.ReadStartElement("SaveOptions");
            //        dum = xreader.ReadElementString("Option");
            //        menuItem22.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("Checked"));
            //        xreader.ReadEndElement();
            //        xreader.ReadStartElement("SaveOptions");
            //        dum = xreader.ReadElementString("Option");
            //        menuItem23.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("Checked"));
            //        xreader.ReadEndElement();
            //        xreader.ReadStartElement("SaveOptions");
            //        dum = xreader.ReadElementString("Option");
            //        menuItem24.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("Checked"));
            //        xreader.ReadEndElement();
            //        xreader.ReadStartElement("SaveOptions");
            //        dum = xreader.ReadElementString("Option");
            //        menuItem25.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("Checked"));
            //        xreader.ReadEndElement();
            //        try
            //        {
            //            xreader.ReadStartElement("SaveOptions");
            //            dum = xreader.ReadElementString("Option");
            //            menuItem29.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("Checked"));
            //            xreader.ReadEndElement();
            //            xreader.ReadStartElement("SaveOptions");
            //            dum = xreader.ReadElementString("Option");
            //            menuItem33.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("Checked"));
            //            xreader.ReadEndElement();
            //            xreader.ReadStartElement("SaveOptions");
            //            dum = xreader.ReadElementString("Option");
            //            menuItem34.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("Checked"));
            //            xreader.ReadEndElement();
            //        }
            //        catch
            //        { };
            //        xreader.ReadEndElement();

            //        xreader.ReadStartElement("Grain-Size");
            //        g1box.Text = xreader.ReadElementString("gs");
            //        gp1box.Text = xreader.ReadElementString("gp");
            //        try
            //        {
            //            suspGS1box.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("ss"));
            //            fallGS1box.Text = xreader.ReadElementString("fv");
            //        }
            //        catch
            //        { };
            //        xreader.ReadEndElement();

            //        xreader.ReadStartElement("Grain-Size");
            //        g2box.Text = xreader.ReadElementString("gs");
            //        gp2box.Text = xreader.ReadElementString("gp");
            //        try
            //        {
            //            suspGS2box.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("ss"));
            //            fallGS2box.Text = xreader.ReadElementString("fv");
            //        }
            //        catch
            //        { };
            //        xreader.ReadEndElement();

            //        xreader.ReadStartElement("Grain-Size");
            //        g3box.Text = xreader.ReadElementString("gs");
            //        gp3box.Text = xreader.ReadElementString("gp");
            //        try
            //        {
            //            suspGS3box.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("ss"));
            //            fallGS3box.Text = xreader.ReadElementString("fv");
            //        }
            //        catch
            //        { };
            //        xreader.ReadEndElement();

            //        xreader.ReadStartElement("Grain-Size");
            //        g4box.Text = xreader.ReadElementString("gs");
            //        gp4box.Text = xreader.ReadElementString("gp");
            //        try
            //        {
            //            suspGS4box.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("ss"));
            //            fallGS4box.Text = xreader.ReadElementString("fv");
            //        }
            //        catch
            //        { };
            //        xreader.ReadEndElement();

            //        xreader.ReadStartElement("Grain-Size");
            //        g5box.Text = xreader.ReadElementString("gs");
            //        gp5box.Text = xreader.ReadElementString("gp");
            //        try
            //        {
            //            suspGS5box.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("ss"));
            //            fallGS5box.Text = xreader.ReadElementString("fv");
            //        }
            //        catch
            //        { };
            //        xreader.ReadEndElement();

            //        xreader.ReadStartElement("Grain-Size");
            //        g6box.Text = xreader.ReadElementString("gs");
            //        gp6box.Text = xreader.ReadElementString("gp");
            //        try
            //        {
            //            suspGS6box.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("ss"));
            //            fallGS6box.Text = xreader.ReadElementString("fv");
            //        }
            //        catch
            //        { };
            //        xreader.ReadEndElement();

            //        xreader.ReadStartElement("Grain-Size");
            //        g7box.Text = xreader.ReadElementString("gs");
            //        gp7box.Text = xreader.ReadElementString("gp");
            //        try
            //        {
            //            suspGS7box.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("ss"));
            //            fallGS7box.Text = xreader.ReadElementString("fv");
            //        }
            //        catch
            //        { };
            //        xreader.ReadEndElement();

            //        xreader.ReadStartElement("Grain-Size");
            //        g8box.Text = xreader.ReadElementString("gs");
            //        gp8box.Text = xreader.ReadElementString("gp");
            //        try
            //        {
            //            suspGS8box.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("ss"));
            //            fallGS8box.Text = xreader.ReadElementString("fv");
            //        }
            //        catch
            //        { };
            //        xreader.ReadEndElement();

            //        try
            //        {
            //            xreader.ReadStartElement("Grain-Size");
            //            g9box.Text = xreader.ReadElementString("gs");
            //            gp9box.Text = xreader.ReadElementString("gp");
            //            try
            //            {
            //                suspGS9box.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("ss"));
            //                fallGS9box.Text = xreader.ReadElementString("fv");
            //            }
            //            catch
            //            { };
            //            xreader.ReadEndElement();
            //        }
            //        catch
            //        { };

            //        xreader.ReadStartElement("File-Parms");

            //        input_time_step_box.Text = xreader.ReadElementString("inputtimestep");
            //        saveintervalbox.Text = xreader.ReadElementString("saveinterval");
            //        outputfilesaveintervalbox.Text = xreader.ReadElementString("savetologfileinterval");
            //        tracerbox.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("tracerrun"));
            //        uniquefilecheck.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("uniquefilecheck"));

            //        xreader.ReadStartElement("Filenames");
            //        dum = xreader.ReadElementString("Desc");
            //        openfiletextbox.Text = xreader.ReadElementString("Name");
            //        xreader.ReadEndElement();
            //        xreader.ReadStartElement("Filenames");
            //        dum = xreader.ReadElementString("Desc");
            //        graindataloadbox.Text = xreader.ReadElementString("Name");
            //        xreader.ReadEndElement();
            //        xreader.ReadStartElement("Filenames");
            //        dum = xreader.ReadElementString("Desc");
            //        bedrockbox.Text = xreader.ReadElementString("Name");
            //        xreader.ReadEndElement();
            //        xreader.ReadStartElement("Filenames");
            //        dum = xreader.ReadElementString("Desc");
            //        raindataloadbox.Text = xreader.ReadElementString("Name");
            //        xreader.ReadEndElement();
            //        xreader.ReadStartElement("Filenames");
            //        dum = xreader.ReadElementString("Desc");
            //        tracerfile.Text = xreader.ReadElementString("Name");
            //        xreader.ReadEndElement();
            //        xreader.ReadStartElement("Filenames");
            //        dum = xreader.ReadElementString("Desc");
            //        tracerhydrofile.Text = xreader.ReadElementString("Name");
            //        xreader.ReadEndElement();
            //        xreader.ReadStartElement("Filenames");
            //        dum = xreader.ReadElementString("Desc");
            //        tracergrainbox.Text = xreader.ReadElementString("Name");
            //        xreader.ReadEndElement();
            //        try
            //        {

            //            xreader.ReadStartElement("Sources");
            //            inbox1.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("input"));
            //            xbox1.Text = xreader.ReadElementString("X");
            //            ybox1.Text = xreader.ReadElementString("Y");
            //            infile1.Text = xreader.ReadElementString("Filename");
            //            xreader.ReadEndElement();
            //            xreader.ReadStartElement("Sources");
            //            inbox2.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("input"));
            //            xbox2.Text = xreader.ReadElementString("X");
            //            ybox2.Text = xreader.ReadElementString("Y");
            //            infile2.Text = xreader.ReadElementString("Filename");
            //            xreader.ReadEndElement();
            //            xreader.ReadStartElement("Sources");
            //            inbox3.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("input"));
            //            xbox3.Text = xreader.ReadElementString("X");
            //            ybox3.Text = xreader.ReadElementString("Y");
            //            infile3.Text = xreader.ReadElementString("Filename");
            //            xreader.ReadEndElement();
            //            xreader.ReadStartElement("Sources");
            //            inbox4.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("input"));
            //            xbox4.Text = xreader.ReadElementString("X");
            //            ybox4.Text = xreader.ReadElementString("Y");
            //            infile4.Text = xreader.ReadElementString("Filename");
            //            xreader.ReadEndElement();
            //            xreader.ReadStartElement("Sources");
            //            inbox5.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("input"));
            //            xbox5.Text = xreader.ReadElementString("X");
            //            ybox5.Text = xreader.ReadElementString("Y");
            //            infile5.Text = xreader.ReadElementString("Filename");
            //            xreader.ReadEndElement();
            //            xreader.ReadStartElement("Sources");
            //            inbox6.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("input"));
            //            xbox6.Text = xreader.ReadElementString("X");
            //            ybox6.Text = xreader.ReadElementString("Y");
            //            infile6.Text = xreader.ReadElementString("Filename");
            //            xreader.ReadEndElement();
            //            xreader.ReadStartElement("Sources");
            //            inbox7.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("input"));
            //            xbox7.Text = xreader.ReadElementString("X");
            //            ybox7.Text = xreader.ReadElementString("Y");
            //            infile7.Text = xreader.ReadElementString("Filename");
            //            xreader.ReadEndElement();
            //            xreader.ReadStartElement("Sources");
            //            inbox8.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("input"));
            //            xbox8.Text = xreader.ReadElementString("X");
            //            ybox8.Text = xreader.ReadElementString("Y");
            //            infile8.Text = xreader.ReadElementString("Filename");
            //            xreader.ReadEndElement();
            //        }
            //        catch
            //        { };

            //        xreader.ReadEndElement();

            //        xreader.ReadStartElement("Description");
            //        DescBox.Text = xreader.ReadElementString("S");
            //        xreader.ReadEndElement();

            //        //JMW 2004-11-11
            //        try
            //        {
            //            xreader.ReadStartElement("OutputFile-Parms");
            //            checkBoxGenerateAVIFile.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("generateavifile"));
            //            textBoxAVIFile.Text = xreader.ReadElementString("avifile");
            //            try
            //            {
            //                saveintervalbox.Text = xreader.ReadElementString("avifreq");
            //                checkBoxGenerateTimeSeries.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("generatetimeseriesfile"));
            //                TimeseriesOutBox.Text = xreader.ReadElementString("timeseriesfile");
            //                outputfilesaveintervalbox.Text = xreader.ReadElementString("timeseriesfreq");
            //                checkBoxGenerateIterations.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("generateiterationsfile"));
            //                IterationOutbox.Text = xreader.ReadElementString("iterationsfile");
            //            }
            //            catch
            //            { };
            //            xreader.ReadEndElement();
            //        }
            //        catch
            //        { };

            //        try
            //        {
            //            xreader.ReadStartElement("Display");
            //            // have to have dumpvariable here as window not displayed yet...
            //            int dumpvarible = XmlConvert.ToInt16(xreader.ReadElementString("top"));
            //            dumpvarible = XmlConvert.ToInt16(xreader.ReadElementString("left"));
            //            dumpvarible = XmlConvert.ToInt16(xreader.ReadElementString("width"));
            //            dumpvarible = XmlConvert.ToInt16(xreader.ReadElementString("height"));
            //            xreader.ReadEndElement();
            //        }
            //        catch
            //        { };

            //        try
            //        {
            //            xreader.ReadStartElement("Lateral");
            //            bool dum_bool = XmlConvert.ToBoolean(xreader.ReadElementString("oldlat"));
            //            newlateral.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("newlat"));
            //            xreader.ReadEndElement();
            //        }
            //        catch
            //        { };
            //        try
            //        {
            //            xreader.ReadStartElement("Add_Ons");
            //            tracerOutcheckBox.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("tracer-out"));
            //            tracerOutputtextBox.Text = xreader.ReadElementString("tracer-out-filename");
            //            googleAnimationCheckbox.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("google_animation"));
            //            googleAnimationTextBox.Text = xreader.ReadElementString("google_animation_file_name");
            //            googleBeginDate.Text = xreader.ReadElementString("google_begin");
            //            googAnimationSaveInterval.Text = xreader.ReadElementString("google_interval");
            //            jmeaninputfilebox.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("jMean"));
            //            avge_smoothbox.Text = xreader.ReadElementString("edge_smoothing");
            //            string dum_string = xreader.ReadElementString("displacement");
            //            propremaining.Text = xreader.ReadElementString("prop_remain");
            //            max_time_step_Box.Text = xreader.ReadElementString("max_time_step");
            //            mine_checkBox.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("contam_input"));
            //            mineX_textBox.Text = xreader.ReadElementString("mineX");
            //            mineY_textBox.Text = xreader.ReadElementString("mineY");
            //            mine_input_textBox.Text = xreader.ReadElementString("contam_input_file");
            //            soil_ratebox.Text = xreader.ReadElementString("soil_rate");
            //            SiberiaBox.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("siberia"));
            //            Beta1Box.Text = xreader.ReadElementString("beta1");
            //            Beta3Box.Text = xreader.ReadElementString("beta3");
            //            m1Box.Text = xreader.ReadElementString("m1");
            //            m3Box.Text = xreader.ReadElementString("m3");
            //            n1Box.Text = xreader.ReadElementString("n1");
            //            Q2box.Text = xreader.ReadElementString("W_depth_erosion_threshold");
            //            dum_string = xreader.ReadElementString("fexp");
            //            div_inputs_box.Text = xreader.ReadElementString("div_inputs");

            //            init_depth_box.Text = xreader.ReadElementString("initial_sand_depth");
            //            slab_depth_box.Text = xreader.ReadElementString("maxslabdepth");
            //            shadow_angle_box.Text = xreader.ReadElementString("angle");
            //            upstream_check_box.Text = xreader.ReadElementString("checkup");
            //            depo_prob_box.Text = xreader.ReadElementString("dep_probability");
            //            offset_box.Text = xreader.ReadElementString("downstream_offset");
            //            dune_time_box.Text = xreader.ReadElementString("dune_timestep");
            //            dune_grid_size_box.Text = xreader.ReadElementString("dune_gridsize");

            //            wilcockbox.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("wilcock"));
            //            einsteinbox.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("einstein"));
            //            DuneBox.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("dune"));

            //            UTMgridcheckbox.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("UTM"));
            //            UTMsouthcheck.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("South"));
            //            UTMzonebox.Text = xreader.ReadElementString("UTMzone");

            //            raintimestepbox.Text = xreader.ReadElementString("raindatatimestep");
            //            activebox.Text = xreader.ReadElementString("activelayerthickness");
 
            //            xreader.ReadEndElement();
            //            xreader.ReadEndElement();
            //        }
            //        catch
            //        { };

            //        xreader.Close();


            //        this.Text = basetext + " (" + Path.GetFileName(cfgname) + ")";
            //        button2.Enabled = true;
            //        start_button.Enabled = false;
            //        Panel1.Visible = false;
            //        tabControl1.Visible = true;

            //    }
            //}

            //////// then initialise

            //int ok;
            //ok = read_header();

            //if (ok == 1)
            //{
            //    initialise();
            //    zero_values();
            //    load_data();

            //    // nActualGridSize 
            //    // moved from initialse() to here MJ 29/03/05 
            //    int x, y;
            //    nActualGridCells = 0;
            //    for (x = 1; x <= xmax; x++)
            //    {
            //        for (y = 1; y <= ymax; y++)
            //        {
            //            if (elev[x, y] > -9999) nActualGridCells++;
            //        }
            //    }

            //    tabControl1.Visible = false;
            //    checkBox1.Checked = false;
            //    zoomPanImageBox1.Visible = true;// MJ 14/01/05
            //    Panel1.Visible = true;						// MJ 14/01/05
            //    button2.Enabled = false;					// MJ 17/01/05
            //    start_button.Enabled = true;				// MJ 17/01/05  
            //    groupBox2.Visible = true;
            //    groupBox3.Visible = true;
            //}

            //// then run program

            //main_loop(this, null);

            ////
            //// end of batch mode section
            //// 
            ////
		}

		void button1_Click(object sender, System.EventArgs e)
		{
            //close google earth animation kml and make kmz
            if (googleAnimationCheckbox.Checked == true)
            {
                StreamWriter kmlsr = File.AppendText(KML_FILE_NAME);
                kml = "\n</Folder>"
                      + "\n</kml>";
                kmlsr.WriteLine(kml);
                kmlsr.Close();
            }

            if (menuItem25.Checked == true) save_data(1, 0); // save waterdepths
            if (menuItem13.Checked == true) save_data(2, 0); // save elevdiff
            if (menuItem12.Checked == true) save_data(3, 0); // save elevations
            if (menuItem14.Checked == true) save_data(4, 0); // save grainsize
            if (menuItem29.Checked == true) save_data(15, 0); // save d50 top layer
            if (menuItem33.Checked == true) save_data(16, 0); // save velocity	<JOE 20050605>
            if (menuItem34.Checked == true) save_data(17, 0); // save soil_saturation	<JOE 20050605>

			this.Close();			
		}
		private void button2_Click(object sender, System.EventArgs e)
		{
            int ok;
            ok = read_header();
            int nnn;
            double temp = -9999;

            if (ok == 1)
            {
                initialise();
                zero_values();
                load_data();

                // nActualGridSize 
                // moved from initialse() to here MJ 29/03/05 
                int x, y;
                //nActualGridCells = 0;
                for (int ii = 1; ii <= rfnum; ii++) nActualGridCells[ii] = 0;

                for (x = 1; x <= xmax; x++)
                {
                    for (y = 1; y <= ymax; y++)
                    {
                        if (elev[x, y] > -9999) nActualGridCells[rfarea[x, y]]++;
                    }
                }

                tabControl1.Visible = false;
                checkBox1.Checked = false;
                zoomPanImageBox1.Visible = true;// MJ 14/01/05
                Panel1.Visible = true;						// MJ 14/01/05
                button2.Enabled = false;					// MJ 17/01/05
                start_button.Enabled = true;				// MJ 17/01/05  
                groupBox2.Visible = true;
                groupBox3.Visible = true;
            }

            string message = "Variables check:";
            if((xmax*ymax)>250000) message +="\n\nWarning, number of cells is greater than 250 000 - this may result in slow model operation";
            if (MIN_Q < (DX / 120)) message += "\n\nWarning, Min_Q may be set too low - suitable value is normally cell size / 100";
            if (MIN_Q > (DX / 80)) message += "\n\nWarning, Min_Q may be set too high - suitable value is normally cell size / 100";
            if (reach_mode_box.Checked == true && inbox1.Checked == false) message += "\n\nWarning, model set to run in reach mode, but no point inputs selected (Hydrology tab)";
            if (reach_mode_box.Checked == true && div_inputs < 0) message += "\n\nWarning, model set to run in reach mode, but divide inputs box (Hydrology tab) set to 0\nit must be 1 or greater";
            if (max_time_step > 3600)
            {
                message += "\n\nMax time step (numerical tab) is set to greater than 3600 - if running in catchment mode\nthis must be smaller than 3600";
                
            }            
            if (water_depth_erosion_threshold > 0.02)
            {
                message += "\n\nWarning, Min depth for erosion threshold (numerical tab) may be set too high";
                message += "\nthis could result in erosion not happening in cells where water depths are low try a value of 0.02 or lower";
            }
            if (water_depth_erosion_threshold < 0.005)
            {
                message += "\n\nWarning, Min depth for erosion threshold (numerical tab) may be set too low";
                message += "\nthis may lead to slow operation as the model tries to erode where very shallow depths ";
            }
            if (d1 > d2 || d2 > d3 || d3 > d4) message += "\n\nWarning, sediment sizes (sediment tab) must be entered in ASCENDING order of size";
            //if (M > 0.1 || M < 0.001) message += "\n\nWarning, M value is unusually high or low. Typical values range from 0.005 to 0.02";
           
            //check for -9999's on RH edge of DEM
            for (nnn = 1; nnn <= ymax; nnn++)
            {
                if (elev[xmax, nnn] > temp) temp = elev[xmax, nnn];
            }
            if (temp < -10)
            {
                message += "\n\nDEM ERROR: CAESAR will not function properly, as the right hand column of the DEM is all nodata (-9999) values. This will prevent any water or sediment from leaving the Rh edge of the model/dem";
            }
            if (edgeslope > 0.01) message += "\nThe edge slope (slope at exit cells for hydraulic model) is probably set too high.. normal values are 0.01 to 0.001";
            //if (bed_proportion > 0.05) message += "\nThe proportion of bedslope erosion is set high - please check";
            //if (bed_proportion > 1) message += "\nProportion of bedlsope erosion is greater than 1 - this must be reduced or the model will not function correctly";
            if (courant_number > 0.7) message += "\nThe courant number is set too high, numerical instabilities are highly likely, it is best set to < 0.5";
            if (courant_number >= 0.4 && DX <= 25) message += "\nThe courant number may be set a little to high for this resolution - consider changing to below 0.4";
            if (courant_number >= 0.3 && DX <= 10) message += "\nThe courant number may be set a little to high for this resolution - consider changing to below 0.3";
            //if (min_time_step <= 0) message += "\nConsider using a minimum time step (e.g. 1 sec or greater) as low time steps can lead to excessive scour during the first few min of model operation";
            if (in_out_difference != 0) message += "\n\nYou have set the input/output difference to be greater than zero, which means the model will speed up/run in steady state  when the difference between water input and output is less than this value"; 
                
            message += "\n\nAll other variables are OK";
            MessageBox.Show(message);

            //main_loop(this, null);
            
		}
		private void textBox2_TextChanged(object sender, System.EventArgs e)
		{
		
		}
		private void contextMenu1_Popup(object sender, System.EventArgs e)
		{
		
		}
        private void popComboBox1()
        {
            if (comboBox1.Items.Count == 1)
            {
                comboBox1.Text = "water depth";
                comboBox1.Text = "erosion/dep";
                comboBox1.Text = "Bed sheer stress";
                comboBox1.Text = "grainsize";
                comboBox1.Text = "tracer";
                comboBox1.Text = "susp conc";
                comboBox1.Text = "soil depth";
                comboBox1.Text = "flow velocity";
            }
        }
		private void menuItem3_Click(object sender, System.EventArgs e)
		{
			menuItem3.Checked=(!menuItem3.Checked);
            if (menuItem3.Checked == true)
            {
                comboBox1.Items.Add("water depth");
            }
            else
            {
                comboBox1.Items.Remove("water depth");
            }
            popComboBox1();
            updateClick = 1;
            this.Refresh();
            drawwater(mygraphics);

		}
		private void menuItem4_Click(object sender, System.EventArgs e)
		{
			menuItem4.Checked=(!menuItem4.Checked);
            if (menuItem4.Checked == true)
            {
                comboBox1.Items.Add("erosion/dep");
            }
            else
            {
                comboBox1.Items.Remove("erosion/dep");
            }
            popComboBox1();
            updateClick = 1;
            this.Refresh();
            drawwater(mygraphics);
		}
		private void menuItem5_Click(object sender, System.EventArgs e)
		{
			menuItem5.Checked=(!menuItem5.Checked);
            updateClick = 1;
            this.Refresh();
            drawwater(mygraphics);
		}
		private void menuItem7_Click(object sender, System.EventArgs e)
		{
			menuItem7.Checked=(!menuItem7.Checked);
            updateClick = 1;
            this.Refresh();
            drawwater(mygraphics);
		}
		private void menuItem8_Click(object sender, System.EventArgs e)
		{
			menuItem8.Checked=(!menuItem8.Checked);
            if (menuItem8.Checked == true)
            {
                comboBox1.Items.Add("Bed sheer stress");
            }
            else
            {
                comboBox1.Items.Remove("Bed sheer stress");
            }
            popComboBox1();
            updateClick = 1;
            this.Refresh();
            drawwater(mygraphics);
		}
		private void menuItem9_Click(object sender, System.EventArgs e)
		{
			menuItem9.Checked=(!menuItem9.Checked);
            if (menuItem9.Checked == true)
            {
                comboBox1.Items.Add("grainsize");
            }
            else
            {
                comboBox1.Items.Remove("grainsize");
            }
            popComboBox1();
            updateClick = 1;
            this.Refresh();
            drawwater(mygraphics);
		}
		private void menuItem12_Click(object sender, System.EventArgs e)
		{
			menuItem12.Checked=(!menuItem12.Checked);
		}
        private void menuItem13_Click(object sender, System.EventArgs e)
		{
			menuItem13.Checked=(!menuItem13.Checked);
		}
		private void menuItem14_Click(object sender, System.EventArgs e)
		{
			menuItem14.Checked=(!menuItem14.Checked);
		}
		private void menuItem25_Click(object sender, System.EventArgs e)
		{
			menuItem25.Checked=(!menuItem25.Checked);
		}
		private void menuItem26_Click(object sender, System.EventArgs e)
		{
			menuItem26.Checked=(!menuItem26.Checked);
            updateClick = 1;
            this.Refresh();
            drawwater(mygraphics);
		}
		private void menuItem27_Click(object sender, System.EventArgs e)
		{
			menuItem27.Checked=(!menuItem27.Checked);
            if (menuItem27.Checked == true)
            {
                comboBox1.Items.Add("susp conc");
            }
            else
            {
                comboBox1.Items.Remove("susp conc");
            }
            popComboBox1();
            updateClick = 1;
            this.Refresh();
            drawwater(mygraphics);
		}
		private void menuItem28_Click(object sender, System.EventArgs e)
		{
			menuItem28.Checked=(!menuItem28.Checked);
            if (menuItem28.Checked == true)
            {
                comboBox1.Items.Add("soil depth");
            }
            else
            {
                comboBox1.Items.Remove("soil depth");
            }
            popComboBox1();
            updateClick = 1;
            this.Refresh();
            drawwater(mygraphics);
        }
		private void menuItem29_Click(object sender, System.EventArgs e)
		{
			menuItem29.Checked=(!menuItem29.Checked);	
		}
		private void menuItem30_Click(object sender, System.EventArgs e)
		{
			menuItem30.Checked=(!menuItem30.Checked);
            updateClick = 1;
            this.Refresh();
            drawwater(mygraphics);
		}
		private void menuItem31_Click(object sender, System.EventArgs e)
		{
			menuItem31.Checked=(!menuItem31.Checked);
            if (menuItem31.Checked == true)
            {
                comboBox1.Items.Add("flow velocity");
            }
            else
            {
                comboBox1.Items.Remove("flow velocity");
            }
            updateClick = 1;
            this.Refresh();
            drawwater(mygraphics);
		}
		private void menuItem33_Click(object sender, System.EventArgs e)
		{
			menuItem33.Checked=(!menuItem33.Checked);	
		}
		private void menuItem34_Click(object sender, System.EventArgs e)
		{
			menuItem34.Checked=(!menuItem34.Checked);	
		}
		private void button3_Click(object sender, System.EventArgs e)
		{
			grow_grass(1);
		}
		private void menuItemConfigFileOpen_Click(object sender, System.EventArgs e)
		{
			XmlTextReader xreader;
			String dum;

			OpenFileDialog openFileDialog1 = new OpenFileDialog();

			openFileDialog1.InitialDirectory = workdir ;
			openFileDialog1.Filter = "cfg files (*.xml)|*.xml|All files (*.*)|*.*" ;
			openFileDialog1.FilterIndex = 1 ;
			openFileDialog1.RestoreDirectory = false ;

			if(openFileDialog1.ShowDialog() == DialogResult.OK)
			{
				cfgname = openFileDialog1.FileName;

				xreader = new XmlTextReader(cfgname);

				//Read the file
				if (xreader != null) 				
				{
					xreader.ReadStartElement("Parms");
					xreader.ReadStartElement("General-Parms");
					try
					{
						overrideheaderBox.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("headeroverride"));
					}
					catch
					{};
					xtextbox.Text = xreader.ReadElementString("x-coordinate");
					ytextbox.Text = xreader.ReadElementString("y-coordinate");
					initscansbox.Text = xreader.ReadElementString("initscans");
					erodefactorbox.Text = xreader.ReadElementString("maxerodelimit");
					dxbox.Text = xreader.ReadElementString("cellsize");
					limitbox.Text = xreader.ReadElementString("memorylimit");
					minqbox.Text = xreader.ReadElementString("minq");
					creepratebox.Text = xreader.ReadElementString("creeprate");
					lateralratebox.Text = xreader.ReadElementString("lateralerosionrate");
					itermaxbox.Text = xreader.ReadElementString("maxiter");
					textBox1.Text = xreader.ReadElementString("runstarttime");
					cyclemaxbox.Text = xreader.ReadElementString("maxrunduration");
					slopebox.Text = xreader.ReadElementString("slopefailurethreshold");
					smoothbox.Text = xreader.ReadElementString("wssmoothingradius");
					mvaluebox.Text = xreader.ReadElementString("mvalue");

					grasstextbox.Text = xreader.ReadElementString("growgrasstime");
					textBox2.Text = xreader.ReadElementString("initialq");
                    try
                    {
                        bool dummy6 = XmlConvert.ToBoolean(xreader.ReadElementString("wssmoothing"));
                    }
                    catch
                    { };
                    flowonlybox.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("grass-sediment"));

					try // MJ 24/01/05
					{
						textBox3.Text = xreader.ReadElementString("flowdistribution");
						mintimestepbox.Text = xreader.ReadElementString("mintimestep");
					}
					catch
					{};

					try // MJ 15/03/05
					{
						k_evapBox.Text = xreader.ReadElementString("evaporation");
					}
					catch
					{};

					try // MJ 10/05/05
					{
						vegTauCritBox.Text = xreader.ReadElementString("vegcritshear");
					}
					catch
					{};

					try 
					{
						bedslope_box.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("bedslope"));
					}
					catch
					{};
					try 
					{
						bool dum_bool = XmlConvert.ToBoolean(xreader.ReadElementString("wsslope"));
					}
					catch
					{};
                    try
                    {
                        veltaubox.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("veltaubox"));
                    }
                    catch
                    { };
                    try 
					{
						catchment_mode_box.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("catchment_mode"));
					}
					catch
					{};
					try 
					{
						reach_mode_box.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("reach_mode"));
					}
					catch
					{};
					try 
					{
						bool dum_bool2 = XmlConvert.ToBoolean(xreader.ReadElementString("lat1"));
					}
					catch
					{};
					try 
					{
                        bool dum_bool2 = XmlConvert.ToBoolean(xreader.ReadElementString("lat2"));
					}
					catch
					{};
					try 
					{
						bool dum_bool = XmlConvert.ToBoolean(xreader.ReadElementString("lat3"));
					}
					catch
					{};
					try 
					{
						string dum_string = xreader.ReadElementString("cross_stream_grad");
					}
					catch
					{};
                    try
                    {
                        max_vel_box.Text = xreader.ReadElementString("max_vel");
                    }
                    catch { };


					xreader.ReadStartElement("SaveOptions");
					dum = xreader.ReadElementString("Option");
					menuItem12.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("Checked"));
					xreader.ReadEndElement();
					xreader.ReadStartElement("SaveOptions");
					dum = xreader.ReadElementString("Option");
					menuItem13.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("Checked"));
					xreader.ReadEndElement();
					xreader.ReadStartElement("SaveOptions");
					dum = xreader.ReadElementString("Option");
					menuItem14.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("Checked"));
					xreader.ReadEndElement();
					xreader.ReadStartElement("SaveOptions");
					dum = xreader.ReadElementString("Option");
					bool dummy4 = XmlConvert.ToBoolean(xreader.ReadElementString("Checked"));
					xreader.ReadEndElement();				    
					xreader.ReadStartElement("SaveOptions");
					dum = xreader.ReadElementString("Option");
                    dummy4 = XmlConvert.ToBoolean(xreader.ReadElementString("Checked"));
					xreader.ReadEndElement();				    
					xreader.ReadStartElement("SaveOptions");
					dum = xreader.ReadElementString("Option");
                    dummy4 = XmlConvert.ToBoolean(xreader.ReadElementString("Checked"));
					xreader.ReadEndElement();				    
					xreader.ReadStartElement("SaveOptions");
					dum = xreader.ReadElementString("Option");
                    dummy4 = XmlConvert.ToBoolean(xreader.ReadElementString("Checked"));
					xreader.ReadEndElement();				    
					xreader.ReadStartElement("SaveOptions");
					dum = xreader.ReadElementString("Option");
                    dummy4 = XmlConvert.ToBoolean(xreader.ReadElementString("Checked"));
					xreader.ReadEndElement();				    
					xreader.ReadStartElement("SaveOptions");
					dum = xreader.ReadElementString("Option");
                    dummy4 = XmlConvert.ToBoolean(xreader.ReadElementString("Checked"));
					xreader.ReadEndElement();				    
					xreader.ReadStartElement("SaveOptions");
					dum = xreader.ReadElementString("Option");
                    dummy4 = XmlConvert.ToBoolean(xreader.ReadElementString("Checked"));
					xreader.ReadEndElement();				    
					xreader.ReadStartElement("SaveOptions");
					dum = xreader.ReadElementString("Option");
                    dummy4 = XmlConvert.ToBoolean(xreader.ReadElementString("Checked"));
					xreader.ReadEndElement();				    
					xreader.ReadStartElement("SaveOptions");
					dum = xreader.ReadElementString("Option");
                    dummy4 = XmlConvert.ToBoolean(xreader.ReadElementString("Checked"));
					xreader.ReadEndElement();				   
					xreader.ReadStartElement("SaveOptions");
					dum = xreader.ReadElementString("Option");
                    dummy4 = XmlConvert.ToBoolean(xreader.ReadElementString("Checked"));
					xreader.ReadEndElement();				    
					xreader.ReadStartElement("SaveOptions");
					dum = xreader.ReadElementString("Option");
					menuItem25.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("Checked"));
					xreader.ReadEndElement();				   
					try
					{
						xreader.ReadStartElement("SaveOptions");
						dum = xreader.ReadElementString("Option");
						menuItem29.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("Checked"));
					xreader.ReadEndElement();
						xreader.ReadStartElement("SaveOptions");
						dum = xreader.ReadElementString("Option");
						menuItem33.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("Checked"));
						xreader.ReadEndElement();				    
						xreader.ReadStartElement("SaveOptions");
						dum = xreader.ReadElementString("Option");
						menuItem34.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("Checked"));
						xreader.ReadEndElement();				    
					}
					catch
					{};
					xreader.ReadEndElement();
				
					xreader.ReadStartElement("Grain-Size");
					g1box.Text = xreader.ReadElementString("gs");
					gp1box.Text = xreader.ReadElementString("gp");
					try
					{
						suspGS1box.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("ss"));
						fallGS1box.Text = xreader.ReadElementString("fv");
					}
					catch
					{};
					xreader.ReadEndElement();

					xreader.ReadStartElement("Grain-Size");
					g2box.Text = xreader.ReadElementString("gs");
					gp2box.Text = xreader.ReadElementString("gp");
					try
					{
						suspGS2box.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("ss"));
						fallGS2box.Text = xreader.ReadElementString("fv");
					}
					catch
					{};
					xreader.ReadEndElement();

					xreader.ReadStartElement("Grain-Size");
					g3box.Text = xreader.ReadElementString("gs");
					gp3box.Text = xreader.ReadElementString("gp");
					try
					{
						suspGS3box.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("ss"));
						fallGS3box.Text = xreader.ReadElementString("fv");
					}
					catch
					{};
					xreader.ReadEndElement();

					xreader.ReadStartElement("Grain-Size");
					g4box.Text = xreader.ReadElementString("gs");
					gp4box.Text = xreader.ReadElementString("gp");
					try
					{
						suspGS4box.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("ss"));
						fallGS4box.Text = xreader.ReadElementString("fv");
					}
					catch
					{};
					xreader.ReadEndElement();

					xreader.ReadStartElement("Grain-Size");
					g5box.Text = xreader.ReadElementString("gs");
					gp5box.Text = xreader.ReadElementString("gp");
					try
					{
						suspGS5box.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("ss"));
						fallGS5box.Text = xreader.ReadElementString("fv");
					}
					catch
					{};
					xreader.ReadEndElement();

					xreader.ReadStartElement("Grain-Size");
					g6box.Text = xreader.ReadElementString("gs");
					gp6box.Text = xreader.ReadElementString("gp");
					try
					{
						suspGS6box.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("ss"));
						fallGS6box.Text = xreader.ReadElementString("fv");
					}
					catch
					{};
					xreader.ReadEndElement();

					xreader.ReadStartElement("Grain-Size");
					g7box.Text = xreader.ReadElementString("gs");
					gp7box.Text = xreader.ReadElementString("gp");
					try
					{
						suspGS7box.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("ss"));
						fallGS7box.Text = xreader.ReadElementString("fv");
					}
					catch
					{};
					xreader.ReadEndElement();

					xreader.ReadStartElement("Grain-Size");
					g8box.Text = xreader.ReadElementString("gs");
					gp8box.Text = xreader.ReadElementString("gp");
					try
					{
						suspGS8box.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("ss"));
						fallGS8box.Text = xreader.ReadElementString("fv");
					}
					catch
					{};
					xreader.ReadEndElement();

					try
					{
						xreader.ReadStartElement("Grain-Size");
						g9box.Text = xreader.ReadElementString("gs");
						gp9box.Text = xreader.ReadElementString("gp");
						try
						{
							suspGS9box.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("ss"));
							fallGS9box.Text = xreader.ReadElementString("fv");
						}
						catch
						{};
						xreader.ReadEndElement();
					}
					catch
					{};

					xreader.ReadStartElement("File-Parms");

					input_time_step_box.Text = xreader.ReadElementString("inputtimestep");
					saveintervalbox.Text = xreader.ReadElementString("saveinterval");
					outputfilesaveintervalbox.Text = xreader.ReadElementString("savetologfileinterval");
					tracerbox.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("tracerrun"));
					uniquefilecheck.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("uniquefilecheck"));

					xreader.ReadStartElement("Filenames");
					dum = xreader.ReadElementString("Desc");
					openfiletextbox.Text = xreader.ReadElementString("Name");
					xreader.ReadEndElement();
					xreader.ReadStartElement("Filenames");
					dum = xreader.ReadElementString("Desc");
					graindataloadbox.Text = xreader.ReadElementString("Name");
					xreader.ReadEndElement();
					xreader.ReadStartElement("Filenames");
					dum = xreader.ReadElementString("Desc");
					bedrockbox.Text = xreader.ReadElementString("Name");
					xreader.ReadEndElement();
					xreader.ReadStartElement("Filenames");
					dum = xreader.ReadElementString("Desc");
					raindataloadbox.Text = xreader.ReadElementString("Name");
					xreader.ReadEndElement();
					xreader.ReadStartElement("Filenames");
					dum = xreader.ReadElementString("Desc");
					tracerfile.Text = xreader.ReadElementString("Name");
					xreader.ReadEndElement();
					xreader.ReadStartElement("Filenames");
					dum = xreader.ReadElementString("Desc");
					tracerhydrofile.Text = xreader.ReadElementString("Name");
					xreader.ReadEndElement();
					xreader.ReadStartElement("Filenames");
					dum = xreader.ReadElementString("Desc");
					string dummystring4 = xreader.ReadElementString("Name");
					xreader.ReadEndElement();
					try{

					xreader.ReadStartElement("Sources");
					inbox1.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("input"));  
					xbox1.Text = xreader.ReadElementString("X");
					ybox1.Text = xreader.ReadElementString("Y");
					infile1.Text = xreader.ReadElementString("Filename");
					xreader.ReadEndElement();
					xreader.ReadStartElement("Sources");
					inbox2.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("input"));  
					xbox2.Text = xreader.ReadElementString("X");
					ybox2.Text = xreader.ReadElementString("Y");
					infile2.Text = xreader.ReadElementString("Filename");
					xreader.ReadEndElement();
					xreader.ReadStartElement("Sources");
					inbox3.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("input"));  
					xbox3.Text = xreader.ReadElementString("X");
					ybox3.Text = xreader.ReadElementString("Y");
					infile3.Text = xreader.ReadElementString("Filename");
					xreader.ReadEndElement();
					xreader.ReadStartElement("Sources");
					inbox4.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("input"));  
					xbox4.Text = xreader.ReadElementString("X");
					ybox4.Text = xreader.ReadElementString("Y");
					infile4.Text = xreader.ReadElementString("Filename");
					xreader.ReadEndElement();
					xreader.ReadStartElement("Sources");
					inbox5.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("input"));  
					xbox5.Text = xreader.ReadElementString("X");
					ybox5.Text = xreader.ReadElementString("Y");
					infile5.Text = xreader.ReadElementString("Filename");
					xreader.ReadEndElement();
					xreader.ReadStartElement("Sources");
					inbox6.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("input"));  
					xbox6.Text = xreader.ReadElementString("X");
					ybox6.Text = xreader.ReadElementString("Y");
					infile6.Text = xreader.ReadElementString("Filename");
					xreader.ReadEndElement();
					xreader.ReadStartElement("Sources");
					inbox7.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("input"));  
					xbox7.Text = xreader.ReadElementString("X");
					ybox7.Text = xreader.ReadElementString("Y");
					infile7.Text = xreader.ReadElementString("Filename");
					xreader.ReadEndElement();
					xreader.ReadStartElement("Sources");
					inbox8.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("input"));  
					xbox8.Text = xreader.ReadElementString("X");
					ybox8.Text = xreader.ReadElementString("Y");
					infile8.Text = xreader.ReadElementString("Filename");
					xreader.ReadEndElement();
					}
					catch
					{};

					xreader.ReadEndElement();

					xreader.ReadStartElement("Description");
					DescBox.Text  = xreader.ReadElementString("S");
					xreader.ReadEndElement();

					//JMW 2004-11-11
					try
					{
						xreader.ReadStartElement("OutputFile-Parms");
                        try
                        {
                            bool a124 = XmlConvert.ToBoolean(xreader.ReadElementString("generateavifile"));
                            string a123 = xreader.ReadElementString("avifile");

                        }
                        catch { };
                        try
						{
							saveintervalbox.Text = xreader.ReadElementString("avifreq");
							checkBoxGenerateTimeSeries.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("generatetimeseriesfile"));
							TimeseriesOutBox.Text = xreader.ReadElementString("timeseriesfile");
							outputfilesaveintervalbox.Text = xreader.ReadElementString("timeseriesfreq");
							checkBoxGenerateIterations.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("generateiterationsfile"));
							IterationOutbox.Text = xreader.ReadElementString("iterationsfile");
						}
						catch
						{};
						xreader.ReadEndElement();
					}
					catch
					{};
					
					try
					{
						xreader.ReadStartElement("Display");
						Form1.ActiveForm.Top = XmlConvert.ToInt16(xreader.ReadElementString("top"));
						Form1.ActiveForm.Left = XmlConvert.ToInt16(xreader.ReadElementString("left"));
						Form1.ActiveForm.Width = XmlConvert.ToInt16(xreader.ReadElementString("width"));
						Form1.ActiveForm.Height = XmlConvert.ToInt16(xreader.ReadElementString("height"));

					xreader.ReadEndElement();
					}
					catch
					{};

					try
					{
						xreader.ReadStartElement("Lateral");
						bool dum_bool = XmlConvert.ToBoolean(xreader.ReadElementString("oldlat"));
						newlateral.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("newlat"));
						xreader.ReadEndElement();
					}
					catch
					{};
                    try
                    {
                        xreader.ReadStartElement("Add_Ons");
                        tracerOutcheckBox.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("tracer-out"));
                        tracerOutputtextBox.Text = xreader.ReadElementString("tracer-out-filename");
                        googleAnimationCheckbox.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("google_animation"));
                        googleAnimationTextBox.Text = xreader.ReadElementString("google_animation_file_name");
                        googleBeginDate.Text = xreader.ReadElementString("google_begin");
                        googAnimationSaveInterval.Text = xreader.ReadElementString("google_interval");
                        jmeaninputfilebox.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("jMean"));
                        avge_smoothbox.Text = xreader.ReadElementString("edge_smoothing");
                        string dum_string = xreader.ReadElementString("displacement");
                        propremaining.Text = xreader.ReadElementString("prop_remain");
                        max_time_step_Box.Text = xreader.ReadElementString("max_time_step");
                        bool dummy1 = XmlConvert.ToBoolean(xreader.ReadElementString("contam_input"));
                        string dummystring1 = xreader.ReadElementString("mineX");
                        string dummystring2 = xreader.ReadElementString("mineY");
                        string dummystring3 = xreader.ReadElementString("contam_input_file");
                        soil_ratebox.Text = xreader.ReadElementString("soil_rate");
                        SiberiaBox.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("siberia"));
                        Beta1Box.Text = xreader.ReadElementString("beta1");
                        Beta3Box.Text = xreader.ReadElementString("beta3");
                        m1Box.Text = xreader.ReadElementString("m1");
                        m3Box.Text = xreader.ReadElementString("m3");
                        n1Box.Text = xreader.ReadElementString("n1");
                        Q2box.Text = xreader.ReadElementString("W_depth_erosion_threshold");
                        dum_string = xreader.ReadElementString("fexp");
                        div_inputs_box.Text = xreader.ReadElementString("div_inputs");

                        init_depth_box.Text = xreader.ReadElementString("initial_sand_depth");
                        slab_depth_box.Text = xreader.ReadElementString("maxslabdepth");
                        shadow_angle_box.Text = xreader.ReadElementString("angle");
                        upstream_check_box.Text = xreader.ReadElementString("checkup");
                        depo_prob_box.Text = xreader.ReadElementString("dep_probability");
                        offset_box.Text = xreader.ReadElementString("downstream_offset");
                        dune_time_box.Text = xreader.ReadElementString("dune_timestep");
                        dune_grid_size_box.Text = xreader.ReadElementString("dune_gridsize");
               
                        wilcockbox.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("wilcock"));
                        einsteinbox.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("einstein"));
                        DuneBox.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("dune"));

                        UTMgridcheckbox.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("UTM"));
                        UTMsouthcheck.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("South"));
                        UTMzonebox.Text = xreader.ReadElementString("UTMzone");

                       
                        raintimestepbox.Text = xreader.ReadElementString("raindatatimestep");
                        activebox.Text = xreader.ReadElementString("activelayerthickness");
                        
                        // more add on's  21/5/2012
                        downstreamshiftbox.Text = xreader.ReadElementString("downstreamshift");
                        courantbox.Text = xreader.ReadElementString("courantnumber");
                        textBox4.Text = xreader.ReadElementString("hflow");
                        textBox7.Text = xreader.ReadElementString("lateralsmoothing");
                        textBox8.Text = xreader.ReadElementString("froude_limit");
                        textBox9.Text = xreader.ReadElementString("mannings");

                        checkBox3.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("TidalorStage"));
                        MinQmaxvalue.Text = xreader.ReadElementString("MinQmaxvalue");
                        TidalXmin.Text = xreader.ReadElementString("TidalXmin");
                        TidalXmax.Text = xreader.ReadElementString("TidalXmax");
                        TidalYmin.Text = xreader.ReadElementString("TidalYmin");
                        TidalYmax.Text = xreader.ReadElementString("TidalYmax");
                        TidalFileName.Text = xreader.ReadElementString("TidalFileName");
                        TidalInputStep.Text = xreader.ReadElementString("TidalInputStep");

                        // more add ons for bedrock erosion 19/1/14
                        bedrock_erosion_threshold_box.Text = xreader.ReadElementString("bedrock_erosion_threshold");
                        bedrock_erosion_rate_box.Text = xreader.ReadElementString("bedrock_erosion_rate");

                        // more add ons for spatially variable rainfall
                        rfnumBox.Text = xreader.ReadElementString("rfnum");
                        hydroindexBox.Text = xreader.ReadElementString("hydroindex");
                        checkBox7.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("MultiRainfall"));

                        // more addons for soil development and spatially variable mannings 28/8/2015
                        soildevbox.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("soildevbox"));
                        checkBox4.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("bedrocklowering"));
                        checkBox5.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("physicalweathering"));
                        checkBox6.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("chemicalweathering"));
                        textBox11.Text = xreader.ReadElementString("P1");
                        textBox12.Text = xreader.ReadElementString("b1");
                        textBox13.Text = xreader.ReadElementString("k1");
                        textBox14.Text = xreader.ReadElementString("c1");
                        textBox15.Text = xreader.ReadElementString("c2");
                        textBox16.Text = xreader.ReadElementString("k2");
                        textBox17.Text = xreader.ReadElementString("c3");
                        textBox18.Text = xreader.ReadElementString("c4");

                        SpatVarManningsCheckbox.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("SpatVarManningsCheckbox"));
                        textBox19.Text = xreader.ReadElementString("spatvarmanningsfilename");
                        mfiletimestepbox.Text = xreader.ReadElementString("mfiletimestepbox");
                        mvalueloadbox.Text = xreader.ReadElementString("mvalueloadbox");

                        // 5/12/16
                        meyerbox.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("meyer"));

                        //18/7/18
                        checkBox8.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("all_nine_grainsizes"));

                        // 11/9/18
						radioButton1.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("oldveg"));
                        radioButton2.Checked = XmlConvert.ToBoolean(xreader.ReadElementString("newveg"));

                        xreader.ReadEndElement();
                        xreader.ReadEndElement();
                    }
                    catch
                    { };
                    

					xreader.Close();

					
					this.Text = basetext + " (" + Path.GetFileName(cfgname) + ")";
					button2.Enabled = true;
					start_button.Enabled = false;
					Panel1.Visible = false;
					tabControl1.Visible = true;
					
				}
			}		
		}
		// JMW - Config File Save & SaveAs Event Handler
		private void menuItemConfigFileSave_Click(object sender, System.EventArgs e)
		{
			XmlTextWriter xwriter;

			if ((sender == menuItemConfigFileSaveAs) || (cfgname == null))
			{
				
				SaveFileDialog saveFileDialog1 = new SaveFileDialog();

				saveFileDialog1.InitialDirectory = workdir ;
				saveFileDialog1.Filter = "cfg files (*.xml)|*.xml|All files (*.*)|*.*" ;
				saveFileDialog1.FilterIndex = 1 ;
				saveFileDialog1.RestoreDirectory = false ;

				if(saveFileDialog1.ShowDialog() == DialogResult.OK)
				{
					cfgname = saveFileDialog1.FileName;
				}

			}

			if (cfgname != null)
			{

				//Create a new XmlTextWriter.
				xwriter = new XmlTextWriter(cfgname,System.Text.Encoding.UTF8);
				//Write the beginning of the document including the 
				//document declaration. Standalone is true. 
				//Use indentation for readability.
				xwriter.Formatting = Formatting.Indented;
				xwriter.Indentation = 4;


				xwriter.WriteStartDocument(true);

				//Write the beginning of the "data" element. This is 
				//the opening tag to our data 
				xwriter.WriteStartElement("Parms");
				xwriter.WriteStartElement("General-Parms");
				xwriter.WriteElementString("headeroverride",XmlConvert.ToString(overrideheaderBox.Checked));
				xwriter.WriteElementString("x-coordinate",xtextbox.Text);				
				xwriter.WriteElementString("y-coordinate",ytextbox.Text);
				xwriter.WriteElementString("initscans",initscansbox.Text);
				xwriter.WriteElementString("maxerodelimit",erodefactorbox.Text);
				xwriter.WriteElementString("cellsize",dxbox.Text);
				xwriter.WriteElementString("memorylimit",limitbox.Text);
				xwriter.WriteElementString("minq",minqbox.Text);
				xwriter.WriteElementString("creeprate",creepratebox.Text);
				xwriter.WriteElementString("lateralerosionrate",lateralratebox.Text);
				xwriter.WriteElementString("maxiter",itermaxbox.Text);
				xwriter.WriteElementString("runstarttime",textBox1.Text);
				xwriter.WriteElementString("maxrunduration",cyclemaxbox.Text);
				xwriter.WriteElementString("slopefailurethreshold",slopebox.Text);
				xwriter.WriteElementString("wssmoothingradius",smoothbox.Text);
				xwriter.WriteElementString("mvalue",mvaluebox.Text);
				xwriter.WriteElementString("growgrasstime",grasstextbox.Text);
				xwriter.WriteElementString("initialq",textBox2.Text);
				xwriter.WriteElementString("wssmoothing","false");
				xwriter.WriteElementString("grass-sediment",XmlConvert.ToString(flowonlybox.Checked));
				xwriter.WriteElementString("flowdistribution",textBox3.Text); // MJ 24/01/05
				xwriter.WriteElementString("mintimestep",mintimestepbox.Text); // MJ 24/01/05
				xwriter.WriteElementString("evaporation",k_evapBox.Text); // MJ 15/03/05
				xwriter.WriteElementString("vegcritshear",vegTauCritBox.Text); // MJ 10/05/05
				xwriter.WriteElementString("bedslope",XmlConvert.ToString(bedslope_box.Checked));
				xwriter.WriteElementString("wsslope",XmlConvert.ToString(false));
                xwriter.WriteElementString("veltaubox", XmlConvert.ToString(veltaubox.Checked));
				xwriter.WriteElementString("catchment_mode",XmlConvert.ToString(catchment_mode_box.Checked));
				xwriter.WriteElementString("reach_mode",XmlConvert.ToString(reach_mode_box.Checked));
				xwriter.WriteElementString("lat1",XmlConvert.ToString(false));
				xwriter.WriteElementString("lat2",XmlConvert.ToString(false));
				xwriter.WriteElementString("lat3",XmlConvert.ToString(false));
                xwriter.WriteElementString("cross_stream_grad", XmlConvert.ToString(0));
                xwriter.WriteElementString("max_vel", max_vel_box.Text);

                
				xwriter.WriteStartElement("SaveOptions");
				xwriter.WriteElementString("Option","elevations");
				xwriter.WriteElementString("Checked",XmlConvert.ToString(menuItem12.Checked));
				xwriter.WriteEndElement();
				xwriter.WriteStartElement("SaveOptions");
				xwriter.WriteElementString("Option","elevdiff");
				xwriter.WriteElementString("Checked",XmlConvert.ToString(menuItem13.Checked));
				xwriter.WriteEndElement();
				xwriter.WriteStartElement("SaveOptions");
				xwriter.WriteElementString("Option","grainsize");
				xwriter.WriteElementString("Checked",XmlConvert.ToString(menuItem14.Checked));
				xwriter.WriteEndElement();
				xwriter.WriteStartElement("SaveOptions");
				xwriter.WriteElementString("Option","total tracer g/s");
				xwriter.WriteElementString("Checked",XmlConvert.ToString(false));
				xwriter.WriteEndElement();
				xwriter.WriteStartElement("SaveOptions");
				xwriter.WriteElementString("Option","tracer layer 1");
				xwriter.WriteElementString("Checked",XmlConvert.ToString(false));
				xwriter.WriteEndElement();
				xwriter.WriteStartElement("SaveOptions");
				xwriter.WriteElementString("Option","tracer layer 2");
                xwriter.WriteElementString("Checked", XmlConvert.ToString(false));
				xwriter.WriteEndElement();
				xwriter.WriteStartElement("SaveOptions");
				xwriter.WriteElementString("Option","tracer layer 3");
                xwriter.WriteElementString("Checked", XmlConvert.ToString(false));
				xwriter.WriteEndElement();
				xwriter.WriteStartElement("SaveOptions");
				xwriter.WriteElementString("Option","tracer layer 4");
                xwriter.WriteElementString("Checked", XmlConvert.ToString(false));
				xwriter.WriteEndElement();
				xwriter.WriteStartElement("SaveOptions");
				xwriter.WriteElementString("Option","tracer layer 5");
                xwriter.WriteElementString("Checked", XmlConvert.ToString(false));
				xwriter.WriteEndElement();
				xwriter.WriteStartElement("SaveOptions");
				xwriter.WriteElementString("Option","tracer layer 6");
                xwriter.WriteElementString("Checked", XmlConvert.ToString(false));
				xwriter.WriteEndElement();
				xwriter.WriteStartElement("SaveOptions");
				xwriter.WriteElementString("Option","tracer layer 7");
                xwriter.WriteElementString("Checked", XmlConvert.ToString(false));
				xwriter.WriteEndElement();
				xwriter.WriteStartElement("SaveOptions");
				xwriter.WriteElementString("Option","tracer layer 8");
                xwriter.WriteElementString("Checked", XmlConvert.ToString(false));
				xwriter.WriteEndElement();
				xwriter.WriteStartElement("SaveOptions");
				xwriter.WriteElementString("Option","tracer layer 9");
                xwriter.WriteElementString("Checked", XmlConvert.ToString(false));
				xwriter.WriteEndElement();
				xwriter.WriteStartElement("SaveOptions");
				xwriter.WriteElementString("Option","water depth");
				xwriter.WriteElementString("Checked",XmlConvert.ToString(menuItem25.Checked));
				xwriter.WriteEndElement();
				xwriter.WriteStartElement("SaveOptions");
				xwriter.WriteElementString("Option","d50");
				xwriter.WriteElementString("Checked",XmlConvert.ToString(menuItem29.Checked));
				xwriter.WriteEndElement();
				xwriter.WriteStartElement("SaveOptions");
				xwriter.WriteElementString("Option","flow velocity");
				xwriter.WriteElementString("Checked",XmlConvert.ToString(menuItem33.Checked));
				xwriter.WriteEndElement();
				xwriter.WriteStartElement("SaveOptions");
				xwriter.WriteElementString("Option","soil saturation");
				xwriter.WriteElementString("Checked",XmlConvert.ToString(menuItem34.Checked));
				xwriter.WriteEndElement();

				xwriter.WriteEndElement();

				xwriter.WriteStartElement("Grain-Size");
				xwriter.WriteElementString("gs",g1box.Text);
				xwriter.WriteElementString("gp",gp1box.Text);
				xwriter.WriteElementString("ss",XmlConvert.ToString(suspGS1box.Checked));					
				xwriter.WriteElementString("fv",fallGS1box.Text);					
				xwriter.WriteEndElement();

				xwriter.WriteStartElement("Grain-Size");
				xwriter.WriteElementString("gs",g2box.Text);
				xwriter.WriteElementString("gp",gp2box.Text);
				xwriter.WriteElementString("ss",XmlConvert.ToString(suspGS2box.Checked));					
				xwriter.WriteElementString("fv",fallGS2box.Text);					
				xwriter.WriteEndElement();
				
				xwriter.WriteStartElement("Grain-Size");
				xwriter.WriteElementString("gs",g3box.Text);				
				xwriter.WriteElementString("gp",gp3box.Text);
				xwriter.WriteElementString("ss",XmlConvert.ToString(suspGS3box.Checked));					
				xwriter.WriteElementString("fv",fallGS3box.Text);					
				xwriter.WriteEndElement();
				
				xwriter.WriteStartElement("Grain-Size");
				xwriter.WriteElementString("gs",g4box.Text);				
				xwriter.WriteElementString("gp",gp4box.Text);
				xwriter.WriteElementString("ss",XmlConvert.ToString(suspGS4box.Checked));					
				xwriter.WriteElementString("fv",fallGS4box.Text);					
				xwriter.WriteEndElement();
				
				xwriter.WriteStartElement("Grain-Size");
				xwriter.WriteElementString("gs",g5box.Text);				
				xwriter.WriteElementString("gp",gp5box.Text);
				xwriter.WriteElementString("ss",XmlConvert.ToString(suspGS5box.Checked));					
				xwriter.WriteElementString("fv",fallGS5box.Text);					
				xwriter.WriteEndElement();
	
				xwriter.WriteStartElement("Grain-Size");
				xwriter.WriteElementString("gs",g6box.Text);				
				xwriter.WriteElementString("gp",gp6box.Text);	
				xwriter.WriteElementString("ss",XmlConvert.ToString(suspGS6box.Checked));					
				xwriter.WriteElementString("fv",fallGS6box.Text);					
				xwriter.WriteEndElement();

				xwriter.WriteStartElement("Grain-Size");
				xwriter.WriteElementString("gs",g7box.Text);				
				xwriter.WriteElementString("gp",gp7box.Text);	
				xwriter.WriteElementString("ss",XmlConvert.ToString(suspGS7box.Checked));					
				xwriter.WriteElementString("fv",fallGS7box.Text);					
				xwriter.WriteEndElement();

				xwriter.WriteStartElement("Grain-Size");
				xwriter.WriteElementString("gs",g8box.Text);				
				xwriter.WriteElementString("gp",gp8box.Text);					
				xwriter.WriteElementString("ss",XmlConvert.ToString(suspGS8box.Checked));					
				xwriter.WriteElementString("fv",fallGS8box.Text);					
				xwriter.WriteEndElement();

				xwriter.WriteStartElement("Grain-Size");
				xwriter.WriteElementString("gs",g9box.Text);				
				xwriter.WriteElementString("gp",gp9box.Text);					
				xwriter.WriteElementString("ss",XmlConvert.ToString(suspGS9box.Checked));					
				xwriter.WriteElementString("fv",fallGS9box.Text);					
				xwriter.WriteEndElement();
                
				xwriter.WriteStartElement("File-Parms");

				xwriter.WriteElementString("inputtimestep",input_time_step_box.Text);
				xwriter.WriteElementString("saveinterval",saveintervalbox.Text);
				xwriter.WriteElementString("savetologfileinterval",outputfilesaveintervalbox.Text);
				xwriter.WriteElementString("tracerrun",XmlConvert.ToString(tracerbox.Checked));
				xwriter.WriteElementString("uniquefilecheck",XmlConvert.ToString(uniquefilecheck.Checked));


				xwriter.WriteStartElement("Filenames");
				xwriter.WriteElementString("Desc","DEM Data File");
				xwriter.WriteElementString("Name",openfiletextbox.Text);
				xwriter.WriteEndElement();
				xwriter.WriteStartElement("Filenames");
				xwriter.WriteElementString("Desc","Grain Data File");
				xwriter.WriteElementString("Name",graindataloadbox.Text);
				xwriter.WriteEndElement();
				xwriter.WriteStartElement("Filenames");
				xwriter.WriteElementString("Desc","Bedrock Data File");
				xwriter.WriteElementString("Name",bedrockbox.Text);
				xwriter.WriteEndElement();
				xwriter.WriteStartElement("Filenames");
				xwriter.WriteElementString("Desc","Rain Data File");
				xwriter.WriteElementString("Name",raindataloadbox.Text);
				xwriter.WriteEndElement();
				xwriter.WriteStartElement("Filenames");
				xwriter.WriteElementString("Desc","Tracer File");
				xwriter.WriteElementString("Name",tracerfile.Text);
				xwriter.WriteEndElement();
				xwriter.WriteStartElement("Filenames");
				xwriter.WriteElementString("Desc","Tracer Sed Vol File");
				xwriter.WriteElementString("Name",tracerhydrofile.Text);
				xwriter.WriteEndElement();
				xwriter.WriteStartElement("Filenames");
				xwriter.WriteElementString("Desc","Tracer Grain Size Data File");
				xwriter.WriteElementString("Name","null");
				xwriter.WriteEndElement();
				xwriter.WriteStartElement("Sources");
				xwriter.WriteElementString("input",XmlConvert.ToString(inbox1.Checked));  
				xwriter.WriteElementString("X",xbox1.Text);
				xwriter.WriteElementString("Y",ybox1.Text);
				xwriter.WriteElementString("Filename",infile1.Text);
				xwriter.WriteEndElement();
				xwriter.WriteStartElement("Sources");
				xwriter.WriteElementString("input",XmlConvert.ToString(inbox2.Checked));  
				xwriter.WriteElementString("X",xbox2.Text);
				xwriter.WriteElementString("Y",ybox2.Text);
				xwriter.WriteElementString("Filename",infile2.Text);
				xwriter.WriteEndElement();
				xwriter.WriteStartElement("Sources");
				xwriter.WriteElementString("input",XmlConvert.ToString(inbox3.Checked));  
				xwriter.WriteElementString("X",xbox3.Text);
				xwriter.WriteElementString("Y",ybox3.Text);
				xwriter.WriteElementString("Filename",infile3.Text);
				xwriter.WriteEndElement();
				xwriter.WriteStartElement("Sources");
				xwriter.WriteElementString("input",XmlConvert.ToString(inbox4.Checked));  
				xwriter.WriteElementString("X",xbox4.Text);
				xwriter.WriteElementString("Y",ybox4.Text);
				xwriter.WriteElementString("Filename",infile4.Text);
				xwriter.WriteEndElement();	
				xwriter.WriteStartElement("Sources");
				xwriter.WriteElementString("input",XmlConvert.ToString(inbox5.Checked));  
				xwriter.WriteElementString("X",xbox5.Text);
				xwriter.WriteElementString("Y",ybox5.Text);
				xwriter.WriteElementString("Filename",infile5.Text);
				xwriter.WriteEndElement();	
				xwriter.WriteStartElement("Sources");
				xwriter.WriteElementString("input",XmlConvert.ToString(inbox6.Checked));  
				xwriter.WriteElementString("X",xbox6.Text);
				xwriter.WriteElementString("Y",ybox6.Text);
				xwriter.WriteElementString("Filename",infile6.Text);
				xwriter.WriteEndElement();	
				xwriter.WriteStartElement("Sources");
				xwriter.WriteElementString("input",XmlConvert.ToString(inbox7.Checked));  
				xwriter.WriteElementString("X",xbox7.Text);
				xwriter.WriteElementString("Y",ybox7.Text);
				xwriter.WriteElementString("Filename",infile7.Text);
				xwriter.WriteEndElement();	
				xwriter.WriteStartElement("Sources");
				xwriter.WriteElementString("input",XmlConvert.ToString(inbox8.Checked));  
				xwriter.WriteElementString("X",xbox8.Text);
				xwriter.WriteElementString("Y",ybox8.Text);
				xwriter.WriteElementString("Filename",infile8.Text);
				xwriter.WriteEndElement();	
				xwriter.WriteEndElement();

				
				xwriter.WriteStartElement("Description");
				xwriter.WriteElementString("S",DescBox.Text);
				xwriter.WriteEndElement();

				//JMW 2004-11-11; updated MJ 24/01/05
				xwriter.WriteStartElement("OutputFile-Parms");
                xwriter.WriteElementString("generateavifile","false");
				xwriter.WriteElementString("avifile","novalue");
				xwriter.WriteElementString("avifreq",saveintervalbox.Text);
				xwriter.WriteElementString("generatetimeseriesfile",XmlConvert.ToString(checkBoxGenerateTimeSeries.Checked));
				xwriter.WriteElementString("timeseriesfile",TimeseriesOutBox.Text);
				xwriter.WriteElementString("timeseriesfreq",outputfilesaveintervalbox.Text);
				xwriter.WriteElementString("generateiterationsfile",XmlConvert.ToString(checkBoxGenerateIterations.Checked));
				xwriter.WriteElementString("iterationsfile",IterationOutbox.Text);
				xwriter.WriteEndElement();


				xwriter.WriteStartElement("Display");
				xwriter.WriteElementString("top",string.Format(" {0}",Form1.ActiveForm.Top));
				xwriter.WriteElementString("left",string.Format(" {0}",Form1.ActiveForm.Left));
				xwriter.WriteElementString("width",string.Format(" {0}",Form1.ActiveForm.Width));
				xwriter.WriteElementString("height",string.Format(" {0}",Form1.ActiveForm.Height));
				xwriter.WriteEndElement();
				
				xwriter.WriteStartElement("Lateral");
				xwriter.WriteElementString("oldlat",XmlConvert.ToString(false));
				xwriter.WriteElementString("newlat",XmlConvert.ToString(newlateral.Checked));
                xwriter.WriteEndElement();

                xwriter.WriteStartElement("Add_Ons");
                xwriter.WriteElementString("tracer-out", XmlConvert.ToString(tracerOutcheckBox.Checked));
                xwriter.WriteElementString("tracer-out-filename", tracerOutputtextBox.Text);
                xwriter.WriteElementString("google_animation", XmlConvert.ToString(googleAnimationCheckbox.Checked));
                xwriter.WriteElementString("google_animation_file_name", googleAnimationTextBox.Text);
                xwriter.WriteElementString("google_begin", googleBeginDate.Text);
                xwriter.WriteElementString("google_interval", googAnimationSaveInterval.Text);
                xwriter.WriteElementString("jMean", XmlConvert.ToString(jmeaninputfilebox.Checked));
                xwriter.WriteElementString("edge_smoothing", avge_smoothbox.Text);
                xwriter.WriteElementString("displacement", XmlConvert.ToString(false));
                xwriter.WriteElementString("prop_remain", propremaining.Text);
                xwriter.WriteElementString("max_time_step", max_time_step_Box.Text);
                xwriter.WriteElementString("contam_input", XmlConvert.ToString(false));
                xwriter.WriteElementString("mineX", "null");
                xwriter.WriteElementString("mineY", "null");
                xwriter.WriteElementString("contam_input_file", "null");
                xwriter.WriteElementString("soil_rate", soil_ratebox.Text);
                xwriter.WriteElementString("siberia", XmlConvert.ToString(SiberiaBox.Checked));
                xwriter.WriteElementString("beta1", Beta1Box.Text);
                xwriter.WriteElementString("beta3", Beta3Box.Text);
                xwriter.WriteElementString("m1", m1Box.Text);
                xwriter.WriteElementString("m3", m3Box.Text);
                xwriter.WriteElementString("n1", n1Box.Text);
                xwriter.WriteElementString("W_depth_erosion_threshold", Q2box.Text);
                xwriter.WriteElementString("fexp",XmlConvert.ToString(1));
                xwriter.WriteElementString("div_inputs",div_inputs_box.Text);
                xwriter.WriteElementString("initial_sand_depth", init_depth_box.Text);
                xwriter.WriteElementString("maxslabdepth", slab_depth_box.Text);
                xwriter.WriteElementString("angle", shadow_angle_box.Text);
                xwriter.WriteElementString("checkup", upstream_check_box.Text);
                xwriter.WriteElementString("dep_probability", depo_prob_box.Text);
                xwriter.WriteElementString("downstream_offset", offset_box.Text);
                xwriter.WriteElementString("dune_timestep", dune_time_box.Text);
                xwriter.WriteElementString("dune_gridsize", dune_grid_size_box.Text);
                xwriter.WriteElementString("wilcock", XmlConvert.ToString(wilcockbox.Checked));
                xwriter.WriteElementString("einstein", XmlConvert.ToString(einsteinbox.Checked));
                xwriter.WriteElementString("dune", XmlConvert.ToString(DuneBox.Checked));
                // three UTM interface elements
                xwriter.WriteElementString("UTM", XmlConvert.ToString(UTMgridcheckbox.Checked));
                xwriter.WriteElementString("South", XmlConvert.ToString(UTMsouthcheck.Checked));
                xwriter.WriteElementString("UTMzone", UTMzonebox.Text);

                xwriter.WriteElementString("raindatatimestep", raintimestepbox.Text);
                xwriter.WriteElementString("activelayerthickness", activebox.Text);

                // more add on's  21/5/2012
 
                xwriter.WriteElementString("downstreamshift", downstreamshiftbox.Text);
                xwriter.WriteElementString("courantnumber", courantbox.Text);
                xwriter.WriteElementString("hflow", textBox4.Text);
                xwriter.WriteElementString("lateralsmoothing", textBox7.Text);
                xwriter.WriteElementString("froude_limit", textBox8.Text);
                xwriter.WriteElementString("mannings", textBox9.Text);

                // more add on's 4/7/13
                xwriter.WriteElementString("TidalorStage", XmlConvert.ToString(checkBox3.Checked));
                xwriter.WriteElementString("MinQmaxvalue", MinQmaxvalue.Text);
                xwriter.WriteElementString("TidalXmin", TidalXmin.Text);
                xwriter.WriteElementString("TidalXmax", TidalXmax.Text);
                xwriter.WriteElementString("TidalYmin", TidalYmin.Text);
                xwriter.WriteElementString("TidalYmax", TidalYmax.Text);
                xwriter.WriteElementString("TidalFileName", TidalFileName.Text);
                xwriter.WriteElementString("TidalInputStep", TidalInputStep.Text);

                // more add ons for bedrock erosion 19/1/14
                xwriter.WriteElementString("bedrock_erosion_threshold", bedrock_erosion_threshold_box.Text);
                xwriter.WriteElementString("bedrock_erosion_rate", bedrock_erosion_rate_box.Text);

                // more add ons for spatially variable rainfall
                xwriter.WriteElementString("rfnum", rfnumBox.Text);
                xwriter.WriteElementString("hydroindex", hydroindexBox.Text);
                xwriter.WriteElementString("MultiRainfall", XmlConvert.ToString(checkBox7.Checked));

                // more addons for soil development and spatially variable mannings 28/8/2015
                xwriter.WriteElementString("soildevbox", XmlConvert.ToString(soildevbox.Checked));
                xwriter.WriteElementString("bedrocklowering", XmlConvert.ToString(checkBox4.Checked));
                xwriter.WriteElementString("physicalweathering", XmlConvert.ToString(checkBox5.Checked));
                xwriter.WriteElementString("chemicalweathering", XmlConvert.ToString(checkBox6.Checked));
                xwriter.WriteElementString("P1", textBox11.Text);
                xwriter.WriteElementString("b1", textBox12.Text);
                xwriter.WriteElementString("k1", textBox13.Text);
                xwriter.WriteElementString("c1", textBox14.Text);
                xwriter.WriteElementString("c2", textBox15.Text);
                xwriter.WriteElementString("k2", textBox16.Text);
                xwriter.WriteElementString("c3", textBox17.Text);
                xwriter.WriteElementString("c4", textBox18.Text);

                xwriter.WriteElementString("SpatVarManningsCheckbox", XmlConvert.ToString(SpatVarManningsCheckbox.Checked));
                xwriter.WriteElementString("spatvarmanningsfilename", textBox19.Text);

                // 4/10/15 spat variable mannings box
                xwriter.WriteElementString("mfiletimestepbox", mfiletimestepbox.Text);
                xwriter.WriteElementString("mvalueloadbox", mvalueloadbox.Text);

                // 5/12/16
                xwriter.WriteElementString("meyer", XmlConvert.ToString(meyerbox.Checked));
                //18/7/18
                xwriter.WriteElementString("all_nine_grainsizes", XmlConvert.ToString(checkBox8.Checked));
				// 11/9/18
				xwriter.WriteElementString("oldveg", XmlConvert.ToString(radioButton1.Checked));
                xwriter.WriteElementString("newveg", XmlConvert.ToString(radioButton2.Checked));

				
                //xwriter.WriteElementString();

                xwriter.WriteEndElement();
				xwriter.WriteEndElement();


				//End the document
				xwriter.WriteEndDocument();

				//Flush the xml document to the underlying stream and
				//close the underlying stream. The data will not be
				//written out to the stream until either the Flush()
				//method is called or the Close() method is called.
				xwriter.Close();

				this.Text = basetext + " (" + Path.GetFileName(cfgname) + ")";
			}
		}
		private void suspCheckedChange(object sender, System.EventArgs e)
		{
			fallGS1box.Enabled = suspGS1box.Checked;
			fallGS2box.Enabled = suspGS2box.Checked;
			fallGS3box.Enabled = suspGS3box.Checked;
			fallGS4box.Enabled = suspGS4box.Checked;
			fallGS5box.Enabled = suspGS5box.Checked;
			fallGS6box.Enabled = suspGS6box.Checked;
			fallGS7box.Enabled = suspGS7box.Checked;
			fallGS8box.Enabled = suspGS8box.Checked;
			fallGS9box.Enabled = suspGS9box.Checked;
		}
		private void fracGSchanged(object sender, System.EventArgs e)
		{
			double sum;

			sum = 0.0;
			if (gp1box.Text != "") sum += double.Parse(gp1box.Text);
			if (gp2box.Text != "") sum += double.Parse(gp2box.Text);
			if (gp3box.Text != "") sum += double.Parse(gp3box.Text);
			if (gp4box.Text != "") sum += double.Parse(gp4box.Text);
			if (gp5box.Text != "") sum += double.Parse(gp5box.Text);
			if (gp6box.Text != "") sum += double.Parse(gp6box.Text);
			if (gp7box.Text != "") sum += double.Parse(gp7box.Text);
			if (gp8box.Text != "") sum += double.Parse(gp8box.Text);
			if (gp9box.Text != "") sum += double.Parse(gp9box.Text);

			if (Math.Abs(sum-1.0) < 0.000000001)
			{
				gpSumLabel.Text = "OK";
				gpSumLabel.ForeColor = Color.Black;
				gpSumLabel2.ForeColor = Color.Black;
			}
			else
			{
				gpSumLabel.Text = string.Format("{0:F8}",sum);
				gpSumLabel.ForeColor = Color.Red;
				gpSumLabel2.ForeColor = Color.Red;
			}
		}
		private void button4_Click_1(object sender, System.EventArgs e)
		{
            updateClick = 1;
			this.Refresh();
			drawwater(mygraphics);
		}
		private void overrideheaderBox_CheckedChanged(object sender, System.EventArgs e)
		{
			label1.Enabled = overrideheaderBox.Checked;
			label2.Enabled = overrideheaderBox.Checked;
			label11.Enabled = overrideheaderBox.Checked;
			xtextbox.Enabled = overrideheaderBox.Checked;
			ytextbox.Enabled = overrideheaderBox.Checked;
			dxbox.Enabled = overrideheaderBox.Checked;
		}
		private void checkBox1_CheckedChanged(object sender, System.EventArgs e)
		{
		
		}
		private void bedslope_box_CheckedChanged(object sender, System.EventArgs e)
		{
			if(bedslope_box.Checked==true){
                veltaubox.Checked=false;
                bedslopebox2.Checked = false;
                 }
		}
		private void newlateral_CheckedChanged(object sender, System.EventArgs e)
		{
			
            if (newlateral.Checked == true) nolateral.Checked = false;
		}
		private void label54_Click(object sender, System.EventArgs e)
		{
		
		}
		private void button5_Click(object sender, System.EventArgs e)
		{
			Form1.ActiveForm.Show();
		}
		private void button5_Click_1(object sender, System.EventArgs e)
		{
            get_area();
		}
        private void veltaubox_CheckedChanged(object sender, EventArgs e)
        {
            if (veltaubox.Checked == true)
            {
                
                bedslope_box.Checked = false;
                bedslopebox2.Checked = false;
            }
        }
        private void backgroundWorker1_DoWork(object sender, DoWorkEventArgs e)
        {
            erodedepo();
        }
        private void nolateral_CheckedChanged(object sender, EventArgs e)
        {
            
            if (nolateral.Checked == true) newlateral.Checked = false;
        }
        private void Form1_Resize(object sender, EventArgs e)
        {
           zoomPanImageBox1.Height = this.Height - 225;
           zoomPanImageBox1.Width = this.Width - 20;
        }
        private void zoomPanImageBox1_Load(object sender, EventArgs e)
        {

        }
        private void trackBar1_Scroll(object sender, EventArgs e)
        {
            contrastMultiplier = contrastFactor[trackBar1.Value];
            drawwater(mygraphics);
        }
        private void trackBar2_Scroll(object sender, EventArgs e)
        {
            magnifyValue = zoomFactor[this.trackBar2.Value];
            zoomPanImageBox1.setZoom();
        }
        private void comboBox1_SelectedValueChanged(object sender, EventArgs e)
        {
            updateClick = 1;
            this.Refresh();
            drawwater(mygraphics);
        }
        private void n1Box_TextChanged(object sender, EventArgs e)
        {

        }
        private void graphicToGoogleEarthButton_Click(object sender, EventArgs e)
        {
            if (coordinateDone == 0)
            {
                //transfrom coordinates
                point testPoint = new point(xll, yll);
                if (UTMgridcheckbox.Checked)
                {
                    testPoint.UTMzone = System.Convert.ToInt32(UTMzonebox.Text);
                    testPoint.south = System.Convert.ToBoolean(UTMsouthcheck.Checked);
                    testPoint.transformUTMPoint();
                }
                else
                {
                testPoint.transformPoint();
                }
                yurcorner = yll + (System.Convert.ToDouble(ymax) * System.Convert.ToDouble(DX));
                xurcorner = xll + (System.Convert.ToDouble(xmax) * System.Convert.ToDouble(DX));
                point testPoint2 = new point(xurcorner, yurcorner);
                if (UTMgridcheckbox.Checked)
                {
                    testPoint2.UTMzone = System.Convert.ToInt32(UTMzonebox.Text);
                    testPoint2.south = System.Convert.ToBoolean(UTMsouthcheck.Checked);
                    testPoint2.transformUTMPoint();
                }
                else
                {
                testPoint2.transformPoint();
                }



                urfinalLati = testPoint2.ycoord;
                urfinalLongi = testPoint2.xcoord;
                llfinalLati = testPoint.ycoord;
                llfinalLongi = testPoint.xcoord;
                coordinateDone = 1;
            }

            //Save image
            m_objDrawingSurface.MakeTransparent();
            m_objDrawingSurface.Save(@"mysavedimage" + imageCount + ".png", System.Drawing.Imaging.ImageFormat.Png);
            //create kml file for image
            string kml_file_name = "image" + imageCount + ".kml";
            StreamWriter kmlsr = File.CreateText(kml_file_name);
            string kml = @"<?xml version=""1.0"" encoding=""UTF-8""?>
                                 <kml xmlns=""http://earth.google.com/kml/2.1"">
                                 <GroundOverlay>
                                 	<name>Untitled Image Overlay</name>";
            kml = kml + "\n<Icon>"
                   + "\n<href>mySavedImage" + imageCount + ".png</href>"
                   + "\n</Icon>"
                   + "\n<LatLonBox>";
            kml = kml + "\n<north>" + urfinalLati + "</north>"
                      + "\n<south>" + llfinalLati + "</south>"
                      + "\n<east>" + urfinalLongi + "</east>"
                      + "\n<west>" + llfinalLongi + "</west>\n";
            kml = kml + @"</LatLonBox>
                                 </GroundOverlay>
                                 </kml>
                                             ";
            kmlsr.Write(kml);
            kmlsr.Close();
            imageCount++;
        }
        private void einsteinbox_CheckedChanged(object sender, EventArgs e)
        {
            if (wilcockbox.Checked == true) wilcockbox.Checked = false;
            if (meyerbox.Checked == true) meyerbox.Checked = false;
        }
        private void wilcockbox_CheckedChanged(object sender, EventArgs e)
        {
            if (einsteinbox.Checked == true) einsteinbox.Checked = false;
            if (meyerbox.Checked == true) meyerbox.Checked = false;
        }
        private void HydrologyTab_Click(object sender, EventArgs e)
        {

        }
        private void button6_Click(object sender, EventArgs e)
        {
            int x, y;
            for (x = 1; x <= xmax; x++)
            {
                for (y = 1; y <= ymax; y++)
                {
                    if (x > 0 && x <= 200) elev[x, y] += 1;
                }
            }
        }
        private void checkBox1_CheckedChanged_1(object sender, EventArgs e)
        {
            if (checkBox1.Checked==false) tabControl1.Visible = false;
            if (checkBox1.Checked == true) tabControl1.Visible = true;
        }
        private void tabPage5_Click(object sender, EventArgs e)
        {

        }
        private void label88_Click(object sender, EventArgs e)
        {

        }
        private void textBox12_TextChanged(object sender, EventArgs e)
        {

        }
        private void label58_Click(object sender, EventArgs e)
        {

        }	 
        private void checkBox4_CheckedChanged(object sender, EventArgs e)
        {
            if (UTMgridcheckbox.Checked)
            {
                UTMzonebox.Visible  = true;
                textBox6.Visible  = true;
                UTMsouthcheck.Visible = true;
                groupBox4.Visible = true;
            }
        }
        private void bedslopebox2_CheckedChanged(object sender, EventArgs e)
        {
            if (bedslopebox2.Checked == true)
            {
                veltaubox.Checked = false;
                bedslope_box.Checked = false;
               
                
            }
        }
        private void UTMgridcheckbox_CheckedChanged(object sender, EventArgs e)
        {
            if (UTMgridcheckbox.Checked)
            {
                UTMzonebox.Visible = true;
                textBox6.Visible = true;
                UTMsouthcheck.Visible = true;
                groupBox4.Visible = true;
            }
            
        }
        private void mouseclick2(object sender, MouseEventArgs e)
        {

        }

        private void checkBox2_CheckedChanged(object sender, EventArgs e)
        {

            if (checkBox2.Checked == true)
            {
                CAESAR_lisflood_1._0.Form2 secondForm = new CAESAR_lisflood_1._0.Form2();
                secondForm.Show();

            }

        }

        private void checkBox7_CheckedChanged(object sender, EventArgs e)
        {
            if (checkBox7.Checked == true)
            {
                rfnumBox.Visible=true;
                hydroindexBox.Visible = true;
                label102.Visible = true;
                label103.Visible = true;
            }
        }

        private void SpatVarManningsCheckbox_CheckedChanged(object sender, EventArgs e)
        {
            if (SpatVarManningsCheckbox.Checked == true)
            {
                textBox19.Visible = true;
                label104.Visible = true;

            }
        }

        private void meyerbox_CheckedChanged(object sender, EventArgs e)
        {
            if (wilcockbox.Checked == true) wilcockbox.Checked = false;
            if (einsteinbox.Checked == true) einsteinbox.Checked = false;
        }

    }

   
    
    class point
    {
        //lat long variables
        public double xcoord;
        public double ycoord;
        public int UTMzone;
        public bool south;
        double transParallelX = 446.448;
        double transParallelY = -125.157;
        double transParallelZ = 542.060;
        double scaleChange = -20.4894 * 0.000001;
        double rotX = (0.1502 / 3600) * (Math.PI / 180);
        double rotY = (0.2470 / 3600) * (Math.PI / 180);
        double rotZ = (0.8421 / 3600) * (Math.PI / 180);
        double a = 6377563.396; //airy 1830 semi-major axis
        double b = 6356256.910; //airy 1830 semi-minor axis
        double a2 = 6378137.000;
        double b2 = 6356752.3142;
        double eSquared = 0;
        double eSquared2 = 0;
        double nO = -100000;//northing of true origin
        double eO = 400000;//easting of true origin
        double fO = 0.9996012717;//scale factor
        double latTrue = 49.0 * (Math.PI / 180.0);//latitude of true origin
        double longTrue = -2.0 * (Math.PI / 180.0);//longitude of true origin
        double psiHash, MBig, v, v2, v3, nLittle, rho, nSquare = 0;
        double vii, viii, ix, Tx2, xi, xii, xiia = 0;
        double helmertX, helmertY, helmertZ, cartX, cartY, cartZ;
        double Height2 = 0;
        double finalLati, finalLongi, latiRad, longiRad = 0;
        double rootXYSqr = 0;
        double PHI1, PHI2, PHI = 0;

        public point(double theXcoord, double theYcoord)//constructor
        {
            this.xcoord = theXcoord;
            this.ycoord = theYcoord;
        }

        public void transformPoint()//british os to lat long
        {
            eSquared = (Math.Pow(a, 2) - Math.Pow(b, 2)) / Math.Pow(a, 2);
            Height2 = 0;

            //OSGB36 easting and northing to OSGB36 latitude and longitude (lower left corner of DTM)
            psiHash = ((this.ycoord - nO) / (a * fO)) + latTrue;
            nLittle = (a - b) / (a + b);
            MBig = b * fO * (((1 + nLittle + ((5.0 / 4.0) * Math.Pow(nLittle, 2)) + ((5.0 / 4.0) * Math.Pow(nLittle, 3))) * (psiHash -
                     latTrue))
               - (((3.0 * nLittle) + (3.0 * Math.Pow(nLittle, 2)) + ((21.0 / 8.0) * Math.Pow(nLittle, 3))) *
                     Math.Sin(psiHash - latTrue) * Math.Cos(psiHash + latTrue))
               + (((15.0 / 8.0 * Math.Pow(nLittle, 2)) + (15.0 / 8.0 * Math.Pow(nLittle, 3))) * Math.Sin(2.0 * (psiHash - latTrue)) * Math.Cos(2.0 * (psiHash + latTrue)))
               - ((35.0 / 24.0 * Math.Pow(nLittle, 3)) * Math.Sin(3.0 * (psiHash - latTrue)) * Math.Cos(3.0 * (psiHash + latTrue))));
            if (Math.Abs((this.ycoord - nO - MBig)) >= 0.01)
            {
                while (Math.Abs((this.ycoord - nO - MBig)) >= 0.01)
                {
                    psiHash = ((this.ycoord - nO - MBig) / (a * fO)) + psiHash;
                    MBig = b * fO * (((1 + nLittle + ((5.0 / 4.0) * Math.Pow(nLittle, 2)) + ((5.0 / 4.0) * Math.Pow(nLittle, 3))) * (psiHash -
                       latTrue))
                 - (((3.0 * nLittle) + (3.0 * Math.Pow(nLittle, 2)) + ((21.0 / 8.0) * Math.Pow(nLittle, 3))) *
                       Math.Sin(psiHash - latTrue) * Math.Cos(psiHash + latTrue))
                 + (((15.0 / 8.0 * Math.Pow(nLittle, 2)) + (15.0 / 8.0 * Math.Pow(nLittle, 3))) * Math.Sin(2.0 * (psiHash - latTrue)) * Math.Cos(2.0 * (psiHash + latTrue)))
                 - ((35.0 / 24.0 * Math.Pow(nLittle, 3)) * Math.Sin(3.0 * (psiHash - latTrue)) * Math.Cos(3.0 * (psiHash + latTrue))));
                }
            }
            v = a * fO * Math.Pow(1 - eSquared * Math.Pow(Math.Sin(psiHash), 2), -.5);
            rho = a * fO * (1 - eSquared) * Math.Pow(1.0 - eSquared * Math.Pow(Math.Sin(psiHash), 2), -1.5);
            nSquare = v / rho - 1.0;
            vii = (Math.Tan(psiHash)) / (2.0 * rho * v);
            viii = ((Math.Tan(psiHash)) / (24.0 * rho * Math.Pow(v, 3))) * (5 + 3.0 * Math.Pow(Math.Tan(psiHash), 2) + nSquare - 9.0 * (Math.Pow(Math.Tan(psiHash), 2) * nSquare));
            ix = (Math.Tan(psiHash) / ((720.0 * rho * Math.Pow(v, 5)))) * (61 + 90.0 * Math.Pow(Math.Tan(psiHash), 2) + 45.0 * Math.Pow(Math.Tan(psiHash), 4));
            Tx2 = (1.0 / Math.Cos(psiHash)) / v;
            xi = (1.0 / Math.Cos(psiHash)) / (6.0 * Math.Pow(v, 3)) * ((v / rho) + (2.0 * Math.Pow(Math.Tan(psiHash), 2)));
            xii = (1.0 / Math.Cos(psiHash)) / (120.0 * Math.Pow(v, 5)) * (5.0 + (28.0 * Math.Pow(Math.Tan(psiHash), 2)) + (24.0 * Math.Pow(Math.Tan(psiHash), 4)));
            xiia = ((1.0 / Math.Cos(psiHash)) / (5040.0 * Math.Pow(v, 7))) * (61.0 + (662.0 * Math.Pow(Math.Tan(psiHash), 2)) + (1320.0 * Math.Pow(Math.Tan(psiHash), 4)) + (720.0 * Math.Pow(Math.Tan(psiHash), 6)));
            latiRad = psiHash - (vii * Math.Pow((this.xcoord - eO), 2)) + (viii * Math.Pow((this.xcoord - eO), 4)) - (ix * Math.Pow((this.xcoord - eO), 6));
            longiRad = longTrue + (Tx2 * (this.xcoord - eO)) - (xi * (Math.Pow((this.xcoord - eO), 3))) + (xii * (Math.Pow((this.xcoord - eO), 5))) - (xiia * (Math.Pow((this.xcoord - eO), 7)));
            //Console.WriteLine(latiRad * (180 / Math.PI));
            //Console.WriteLine(longiRad * (180 / Math.PI));
            //OSGB36 Latitude Longitude Height to OSGB36 Cartesian XYZ
            v2 = a / (Math.Sqrt(1 - (eSquared * ((Math.Pow(Math.Sin(latiRad), 2))))));
            cartX = (v2 + Height2) * Math.Cos(latiRad) * Math.Cos(longiRad);
            cartY = (v2 + Height2) * (Math.Cos(latiRad) * Math.Sin(longiRad));
            cartZ = ((v2 * (1 - eSquared)) + Height2) * Math.Sin(latiRad);
            //Console.WriteLine();
            //Console.WriteLine(cartX);
            //Console.WriteLine(cartY);
            //Helmert Datum Transformation (OSGB36 to WGS84)
            helmertX = cartX + (cartX * scaleChange) - (cartY * rotZ) + (cartZ * rotY) + transParallelX;
            helmertY = (cartX * rotZ) + cartY + (cartY * scaleChange) - (cartZ * rotX) + transParallelY;
            helmertZ = (-1 * cartX * rotY) + (cartY * rotX) + cartZ + (cartZ * scaleChange) + transParallelZ;
            //Console.WriteLine();
            //Console.WriteLine(helmertX);
            //Console.WriteLine(helmertY);
            //WGS84 Cartesian XYZ to WGS84 Latitude, longitude and Ellipsoidal height								
            rootXYSqr = Math.Sqrt((Math.Pow(helmertX, 2)) + (Math.Pow(helmertY, 2)));
            eSquared2 = (Math.Pow(a2, 2) - Math.Pow(b2, 2)) / Math.Pow(a2, 2);
            PHI1 = Math.Atan(helmertZ / (rootXYSqr * (1 - eSquared2)));
            v3 = a2 / (Math.Sqrt(1.0 - (eSquared2 * ((Math.Pow(Math.Sin(PHI1), 2))))));
            PHI2 = Math.Atan((helmertZ + (eSquared2 * v3 * (Math.Sin(PHI1)))) / rootXYSqr);
            while (Math.Abs(PHI1 - PHI2) > 0.000000001)
            {
                PHI1 = PHI2;
                v3 = a2 / (Math.Sqrt(1 - (eSquared2 * ((Math.Pow(Math.Sin(PHI1), 2))))));
                PHI2 = Math.Atan((helmertZ + (eSquared2 * v3 * (Math.Sin(PHI1)))) / rootXYSqr);
            }
            PHI = PHI2;
            finalLati = PHI * (180.0 / Math.PI);
            finalLongi = (Math.Atan(helmertY / helmertX)) * (180.0 / Math.PI);
            this.xcoord = finalLongi;
            this.ycoord = finalLati;
        }

        public void transformUTMPoint()  
        {
            //transforms coordinates in UTM WGS84 to lat long
            //requires x, y, zone and north/south
            //the code in this function was found at http://home.hiwaay.net/~taylorc/toolbox/geography/geoutm.html
            //made by Chuck Taylor
            //tested in xls for points in Poland, Turkey and South Africa
            
            // The code first calculates TM coordinates from UTM coordinates
            // Then calculates corresponding latitude and longitude in radians
            // Before converting back to degrees
            
            // first version ArT 12-06-09 

            double footpointlatitude = 0;
            double UTMscalefactor = 0.9996;
            double centralmeridian_deg = 0;
            double centralmeridian_rad = 0;
            double y_ = 0;
            double WGS84_sm_a = 6378137;
            double WGS84_sm_b = 6356752.314;
            double n = (WGS84_sm_a-WGS84_sm_b) / (WGS84_sm_a+WGS84_sm_b);

            this.xcoord = (this.xcoord - 500000) / UTMscalefactor;
            if (this.south) this.ycoord = (this.ycoord - 10000000) / UTMscalefactor;
            else this.ycoord /= UTMscalefactor;

            centralmeridian_deg = -183 + (this.UTMzone * 6);
            centralmeridian_rad = centralmeridian_deg / 180 * Math.PI;

            double alpha = (((WGS84_sm_a+WGS84_sm_b)/2)*(1+Math.Pow(n,2)/4)+(Math.Pow(n,4)/64));
            double beta = (3*n/2)+(-27*Math.Pow(n,3)/32)+(269*Math.Pow(n,5)/512);
            double gamma = (21*Math.Pow(n,2)/16)+(-55*Math.Pow(n,4)/32);
            double delta = (151*Math.Pow(n,3)/96)+(-417*Math.Pow(n,5)/128); 
            double epsilon = (1097*Math.Pow(n,4)/512);

            y_ = this.ycoord / (alpha);
            footpointlatitude = y_ + (beta*Math.Sin(2*y_)) + (gamma*Math.Sin(4*y_)) + (delta*Math.Sin(6*y_)) + (epsilon*Math.Sin(8*y_));

            double ep2 = (Math.Pow(WGS84_sm_a,2)-Math.Pow(WGS84_sm_b,2)) / Math.Pow(WGS84_sm_b,2);
            double cf = Math.Cos(footpointlatitude);
            double nuf2 = ep2*Math.Pow(cf,2);
            double nf = Math.Pow(WGS84_sm_a,2)/(WGS84_sm_b*Math.Sqrt(1+nuf2));

            double tf = Math.Tan(footpointlatitude);
            double tf2 = Math.Pow(tf,2);
            double tf4 = Math.Pow(tf,4);

            double x1frac = 1  /(1*    Math.Pow(nf,1) * cf);
            double x2frac = tf /(2*    Math.Pow(nf,2));
            double x3frac = 1  /(6*    Math.Pow(nf,3) * cf);
            double x4frac = tf /(24*   Math.Pow(nf,4));
            double x5frac = 1  /(120*  Math.Pow(nf,5) * cf);
            double x6frac = tf /(720*  Math.Pow(nf,6));
            double x7frac = 1  /(5040* Math.Pow(nf,7) * cf);
            double x8frac = tf /(40320*Math.Pow(nf,8)); 

            double x2poly =   -1 - nuf2;
            double x3poly =   -1 - nuf2 - (2*tf2);
            double x4poly =    5 + 3*tf2+6*nuf2-6*tf2*nuf2-3*Math.Pow(nuf2,2)-9*tf2*nuf2*nuf2;
            double x5poly =    5 + 28*tf2+24*tf4+6*nuf2+8*tf2*nuf2;
            double x6poly =  -61 - 90*tf2-45*tf4-107*nuf2+162*tf2*nuf2;
            double x7poly =  -61 - 662*tf2-1320*tf4-720*tf4*tf2;
            double x8poly = 1385 + 3633*tf2+4095*tf4+1575*tf4*tf2;

            double latitude = footpointlatitude 
                            + x2frac*x2poly*Math.Pow(this.xcoord,2)
                            + x4frac*x4poly*Math.Pow(this.xcoord,4)
                            + x6frac*x6poly*Math.Pow(this.xcoord,6)
                            + x8frac*x8poly*Math.Pow(this.xcoord,8);
            double longitude = centralmeridian_rad
                            + x1frac * 1   *Math.Pow(this.xcoord,1)
                            + x3frac*x3poly*Math.Pow(this.xcoord,3)
                            + x5frac*x5poly*Math.Pow(this.xcoord,5)
                            + x7frac*x7poly*Math.Pow(this.xcoord,7);

            this.ycoord = latitude / Math.PI * 180;
            this.xcoord = longitude / Math.PI * 180;
        }
        
    }


}




