// *************************************************************************
//
// Copyright 2004-2010 Bruno PAGES  .
// Copyright 2012-2013 Nikolai Marchenko.
// Copyright 2012-2013 Leonardo Guilherme.
//
// This file is part of the DOUML Uml Toolkit.
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License Version 3.0 as published by
// the Free Software Foundation and appearing in the file LICENSE.GPL included in the
//  packaging of this file.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License Version 3.0 for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//
// e-mail : doumleditor@gmail.com
// home   : http://sourceforge.net/projects/douml
//
// *************************************************************************

#ifndef SDDURATIONCANVAS_H
#define SDDURATIONCANVAS_H

#include "SdMsgSupport.h"
#include "SdDurationSupport.h"
//Added by qt3to4:
#include <QTextStream>

#define DURATION_WIDTH 11

class SdObjCanvas;
class SdLifeLineCanvas;
class SdMsgBaseCanvas;
class ToolCom;

class SdDurationCanvas : public QObject, public SdMsgSupport, public SdDurationSupport
{
    Q_OBJECT

protected:
    SdDurationSupport * support;
    QList<SdDurationCanvas *> durations;
    QList<SdMsgBaseCanvas *> msgs;
    UmlColor itscolor;
    bool coregion;

    SdDurationCanvas(UmlCanvas * canvas, SdDurationSupport * sp,
                     int x, int y, int w, int h, int id, bool coreg);

    void save_sub(QTextStream &) const;
    void save_internal(QTextStream &) const;
    void cut_internal(int py);
    void update_self();
    void cut(const QPoint & p);
    void merge(QList<SdDurationCanvas *> &);
    void collapse(SdDurationCanvas *);
    void toOverlapping(SdMsgBaseCanvas **, SdDurationCanvas * orig,
                       unsigned & index, unsigned sz);
    void postToOverlapping();

    static SdDurationCanvas * read_internal(char *& st, UmlCanvas *, int id, SdDurationSupport *);

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
public:
    SdDurationCanvas(UmlCanvas * canvas, SdDurationSupport * sp,
                     int v, bool isdest);
    virtual ~SdDurationCanvas();

    virtual void delete_it() override;

    virtual void add(SdMsgBaseCanvas *) override;
    virtual void remove(SdMsgBaseCanvas *) override;
    virtual void add(SdDurationCanvas *) override;
    using SdDurationSupport::remove;
    using SdMsgSupport::remove;
    virtual void remove(SdDurationCanvas *) override;
    virtual void update_instance_dead() override;
    virtual void update_v_to_contain(SdDurationCanvas *, bool force) override;
    virtual int sub_x(int sub_w) const override;
    virtual double min_y() const override;
    virtual SdLifeLineCanvas * get_line() const override;
    virtual bool isaDuration() const override;
    virtual bool isOverlappingDuration() const override;
    virtual double getZ() const override;
    void go_up(SdMsgBaseCanvas *, bool isdest);
    void go_down(SdMsgBaseCanvas *);
    void update_hpos();
    virtual void update_v_to_contain(const QRect re) override;
    void toFlat();
    void toOverlapping();

    virtual void draw(QPainter & p);
    virtual void moveBy(double dx, double dy) override;
    virtual void prepare_for_move(bool on_resize) override;

    virtual UmlCode typeUmlCode() const override;
    virtual void open() override;
    virtual void menu(const QPoint &) override;
    virtual QString may_start(UmlCode &) const override;
    virtual QString may_connect(UmlCode & l, const DiagramItem * dest) const override;
    virtual bool may_connect(UmlCode l) const override;
    virtual aCorner on_resize_point(const QPoint &) override;
    virtual void resize(int w, int h) override;
    virtual void resize(aCorner c, int dx, int dy, QPoint &) override;
    virtual void resize(const QSize & sz, bool w, bool h) override;
    virtual void change_scale() override;
    virtual void connexion(UmlCode, DiagramItem *, const QPoint &, const QPoint &) override;
    virtual bool connexion(UmlCode, const QPoint &, const QPoint &) override;
    virtual void delete_available(BooL & in_model, BooL & out_model) const override;
    virtual LineDirection allowed_direction(UmlCode) override;
    virtual void select_associated() override;
    virtual bool copyable() const override;
    virtual void history_save(QBuffer & b) const override;
    virtual void history_load(QBuffer &) override;
    virtual void history_hide() override;

    virtual bool has_drawing_settings() const override;
    virtual void edit_drawing_settings(QList<DiagramItem *> &) override;
    virtual void clone_drawing_settings(const DiagramItem *src) override;

    virtual void apply_shortcut(const QString & s) override;
    void edit_drawing_settings();

    virtual void save(QTextStream &, bool ref, QString & warning) const override;
    static SdDurationCanvas * read(char *& st, UmlCanvas * canvas, char *);
    static SdDurationCanvas * read(char *& st, UmlCanvas * canvas, bool ref);

    unsigned count_msg(int api_format) const;
    void send(ToolCom * com, int id) const;

    static void propag_visible(QList<SdDurationCanvas *> &, bool y);
    static void propag_dz(QList<SdDurationCanvas *> &, double dz);

private slots:
    void modified();
};

#endif
