/*
 * Copyright (C) 2004 Ivo Danihelka (ivo@danihelka.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
#include "MarkMask.h"

#include "Cube.h"
#include "Shape.h"
#include "Field.h"

#include <algorithm>

//-----------------------------------------------------------------
MarkMask::MarkMask(Cube *model, Field *field)
{
    m_model = model;
    m_field = field;
}
//-----------------------------------------------------------------
/**
 * Return others which resist us to move this direction.
 * Pointers to NULL and own model are not included.
 *
 * @return unique pointers, not NULL
 */
Cube::t_models
MarkMask::getResist(Dir::eDir dir) const
{
    V2 shift = Dir::dir2xy(dir);
    V2 shift_loc = shift.plus(m_model->getLocation());

    return getPlacedResist(shift_loc);
}
//-----------------------------------------------------------------
/**
 * Return others which resist at given location.
 * Pointers to NULL and own model are not included.
 *
 * @return unique pointers, not NULL
 */
Cube::t_models
MarkMask::getPlacedResist(const V2 &loc) const
{
    Cube::t_models models;
    const Shape *shape = m_model->shape();
    Shape::const_iterator end = shape->marksEnd();
    for (Shape::const_iterator i = shape->marksBegin(); i != end; ++i) {
        V2 mark = loc.plus(*i);

        Cube *resist = m_field->getModel(mark);
        if (NULL != resist && m_model != resist) {
            models.push_back(resist);
        }
    }

    std::sort(models.begin(), models.end());
    Cube::t_models::iterator last = std::unique(models.begin(), models.end());
    models.erase(last, models.end());

    return models;
}
//-----------------------------------------------------------------
/**
 * Write our position to the field.
 */
void
MarkMask::mask()
{
    writeModel(m_model);
}
//-----------------------------------------------------------------
/**
 * Clear our position from the field.
 */
void
MarkMask::unmask()
{
    writeModel(NULL);
}
//-----------------------------------------------------------------
void
MarkMask::writeModel(Cube *model)
{
    V2 loc = m_model->getLocation();
    const Shape *shape = m_model->shape();
    Shape::const_iterator end = shape->marksEnd();
    for (Shape::const_iterator i = shape->marksBegin(); i != end; ++i) {
        V2 mark = loc.plus(*i);
        m_field->setModel(mark, model);
    }
}

//-----------------------------------------------------------------
/**
 * Returns dir to out of room.
 * The direction must be without resist.
 * @return return dir or DIR_NO when model is not at the border.
 */
    Dir::eDir
MarkMask::getBorderDir() const
{
    V2 loc = m_model->getLocation();
    const Shape *shape = m_model->shape();
    Shape::const_iterator end = shape->marksEnd();
    for (Shape::const_iterator i = shape->marksBegin(); i != end; ++i) {
        V2 mark = loc.plus(*i);
        if (mark.getX() == 0 && isBorderDir(Dir::DIR_LEFT)) {
            return Dir::DIR_LEFT;
        }
        else if (mark.getX() == m_field->getW() - 1 &&
                isBorderDir(Dir::DIR_RIGHT))
        {
            return Dir::DIR_RIGHT;
        }
        else if (mark.getY() == 0 && isBorderDir(Dir::DIR_UP)) {
            return Dir::DIR_UP;
        }
        else if (mark.getY() == m_field->getH() - 1 &&
                isBorderDir(Dir::DIR_DOWN))
        {
            return Dir::DIR_DOWN;
        }
    }

    return Dir::DIR_NO;
}
//-----------------------------------------------------------------
/**
 * Test whether there is only border in this direction.
 */
    bool
MarkMask::isBorderDir(Dir::eDir dir) const
{
    bool result = false;
    Cube::t_models resist = getResist(dir);
    if (resist.size() == 1) {
        if (resist[0]->getIndex() == -1) {
            result = true;
        }
    }
    return result;
}

