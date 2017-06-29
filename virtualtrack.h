#ifndef VIRTUALTRACK_H
#define VIRTUALTRACK_H

/*!
 * \brief The VirtualTrack class is implemented by other possible trackers.
 */
class VirtualTrack
{
    /*!
     * \brief Track cars in the scene
     */
    virtual void process() = 0;
};

#endif // VIRTUALTRACK_H
