/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt Charts module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

import QtQuick 2.0
import QtQuick.Controls 2.1
import QtCharts 2.0

Item {
    anchors.fill: parent


    ChartView {
          title: "Bar series"
          anchors.fill: parent
          legend.alignment: Qt.AlignBottom
          antialiasing: true

          BarSeries {
              id: mySeries2
              axisX: BarCategoryAxis { categories: ["2007", "2008", "2009", "2010", "2011", "2012" ] }
              BarSet { label: "Bob"; values: [2, 2, 3, 4, 5, 6] }

          }
      }
//    // Add data dynamically to the series
//    Component.onCompleted: {
//        for (var i = 0; i <= 10; i++) {
//            series1.append(i, Math.random());
//            series2.append(i, Math.random());
//        }
//    }
    //![1]
}
