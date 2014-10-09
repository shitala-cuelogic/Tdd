<?php
/**
 * Sending email function with attachment
 * @param string $strSendName : Sender Name
 * @param string $strSendEmail : Sender Email
 * @param string $strRecipient : Recipient Email
 * @param string $strSubject : Email Subject
 * @param string $strMessage : Email Message
 * @param string $arrAttachment : Email Attachment
 * @param bool $boolTest : Flag to check test email
 * @return mixed
 */
function fnSendEmail($strSendName = '3blmedia Team', $strSendEmail = '', $strRecipient = '', $strSubject = '', $strMessage = '', $arrAttachment = '', $boolTest = false)
{

    include_once libraries_get_path('PHPMailer') . '/class.phpmailer.php';
    //creating object
    $objMail = new PHPMailer(true); //New instance, with exceptions enabled

    // Here's the code that allows special chars in subject and body
    $objMail->CharSet  = 'UTF-8';
    $objMail->Encoding = 'quoted-printable';

    $strMessage = @eregi_replace("[\]", '', $strMessage);
    $strSubject = @eregi_replace("[\]", '', $strSubject);

    //Message of Mail
    $strBody             = $strMessage;
    $strBody             = preg_replace('/\\\\/', '', $strBody); //Strip backslashes

    $objMail->IsSendmail();  // tell the class to use Send-mail
    $objMail->From       = $strSendEmail; //Sender email
    $objMail->FromName   = $strSendName;  //Sender Name

    //Getting all emails.
    $arrRecipient = explode(",", $strRecipient);
    if ($boolTest == true) {
        /*$objMail->AddAddress("dfulton@3blmedia.com");
        $objMail->AddAddress("dsayers@3blmedia.com");
        $objMail->AddAddress("reggie@3blmedia.com");*/
        $objMail->AddAddress("tushar.mate@cuelogic.co.in");
        $objMail->AddAddress("prasanaa.cuelogic@gmail.com");

    } else {
        if (is_array($arrRecipient)) {
            foreach ($arrRecipient as $strToEmail) {
                if ($strToEmail !="") {
                    $objMail->AddAddress($strToEmail);  // Recipient (to)
                } else {
                    return false;
                }
            }
        }
    }

    // $objMail->AddAddress($strRecipient);  // Recipient (to)
    $objMail->Subject  = $strSubject;     //Subject

    $objMail->MsgHTML($strBody); // Body

    //Using attachment file.
    if ($arrAttachment!='') {
        //checking array exist or not.
        if (is_array($arrAttachment)) {

            //for each attachment
            foreach ($arrAttachment as $strAttachment) {
                $objMail->AddAttachment($strAttachment);
            }//foreach

        } else {
            //send one attachment.
            $objMail->AddAttachment($arrAttachment);
        }
    }//attachment is empty.
    $strEmailCheck = $objMail->Send();
    return $strEmailCheck;
}//end function


