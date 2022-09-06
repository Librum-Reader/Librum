/*
    SPDX-FileCopyrightText: 2012 Fabio D 'Urso <fabiodurso@hotmail.it>

    Work sponsored by the LiMux project of the city of Munich:
    SPDX-FileCopyrightText: 2017 Klarälvdalens Datakonsult AB a KDAB Group company <info@kdab.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef _OKULAR_GENERATOR_PDF_ANNOTS_H_
#define _OKULAR_GENERATOR_PDF_ANNOTS_H_

#include <poppler-annotation.h>
#include <poppler-qt5.h>
#include <poppler-version.h>
#define POPPLER_VERSION_MACRO ((POPPLER_VERSION_MAJOR << 16) | (POPPLER_VERSION_MINOR << 8) | (POPPLER_VERSION_MICRO))

#include <QMutex>

#include <unordered_map>

#include "core/annotations.h"

extern Okular::Annotation *createAnnotationFromPopplerAnnotation(Poppler::Annotation *popplerAnnotation, const Poppler::Page &popplerPage, bool *doDelete);

class PopplerAnnotationProxy : public Okular::AnnotationProxy
{
public:
    PopplerAnnotationProxy(Poppler::Document *doc, QMutex *userMutex, QHash<Okular::Annotation *, Poppler::Annotation *> *annotsOnOpenHash);
    ~PopplerAnnotationProxy() override;

    bool supports(Capability capability) const override;
    void notifyAddition(Okular::Annotation *okl_ann, int page) override;
    void notifyModification(const Okular::Annotation *okl_ann, int page, bool appearanceChanged) override;
    void notifyRemoval(Okular::Annotation *okl_ann, int page) override;

private:
    Poppler::Document *ppl_doc;
    QMutex *mutex;
    QHash<Okular::Annotation *, Poppler::Annotation *> *annotationsOnOpenHash;
#if POPPLER_VERSION_MACRO >= QT_VERSION_CHECK(21, 10, 0)
    std::unordered_map<Okular::StampAnnotation *, std::unique_ptr<Poppler::AnnotationAppearance>> deletedStampsAnnotationAppearance;
#endif
};

#endif
